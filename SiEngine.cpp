#include "SiEngine.h"
#include <assert.h>

WinApp* winApp_ = new WinApp;
WinApp* SiEngine::win_;
DirectXCommon* SiEngine::dxCommon_;

void SiEngine::Initialize(WinApp* win, const wchar_t* title, int32_t width, int32_t height)
{
	dxCommon_->Initialization(win, title, win->GetkClientWidth(), win->GetkClientHeight());
	dxCommon_->ImGuiInitialize();
	InitializeDxcCompiler();
	CreateRootSignature();
	CreateInputlayOut();
	BlendState();
	RasterizerState();
	InitializePSO();
	ViewPort();
	ScissorRect();
	
}

IDxcBlob* SiEngine::CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler)
{
	//これからシェーダーをコンパイルする旨をログに出す
	Log(ConvertString(std::format(L"Begin CompileShader, path:{},profile:{}\n", filePath, profile)));

	//hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	dxCommon_->SetHr(dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource));

	//読めなかったら決める
	assert(SUCCEEDED(dxCommon_->GetHr()));

	//読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;
	LPCWSTR arguments[] = {
		filePath.c_str(),//コンパイル対象のhlslファイル名
		L"-E",L"main",//エントリーポイントの指定。基本的にmain以外にはしない
		L"-T",profile,//ShaderProflieの設定
		L"-Zi",L"-Qembed_debug",//デバッグ用の情報を埋め込む
		L"-Od", //最適化を外しておく
		L"-Zpr",//メモリレイアウトは行優先
	};

	//実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	dxCommon_->SetHr(dxcCompiler->Compile(
		&shaderSourceBuffer,//読み込んだファイル
		arguments,//コンパイルオプション
		_countof(arguments),//コンパイルオプションの数
		includeHandler, // includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult)//コンパイル結果
	));

	//コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(dxCommon_->GetHr()));

	//警告・エラーが出たらログに出して止める
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0)
	{
		Log(shaderError->GetStringPointer());
		//警告・エラーダメ絶対
		assert(false);
	}

	//コンパイル結果から実行用のバイナリ部分を取得
	IDxcBlob* shaderBlob = nullptr;
	dxCommon_->SetHr(shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr));
	assert(SUCCEEDED(dxCommon_->GetHr()));

	//成功したログを出す
	Log(ConvertString(std::format(L"Compile Succeeded, path:{},profile:{}\n", filePath, profile)));

	//もう使わないリソースを開放
	shaderSource->Release();
	shaderResult->Release();

	//実行用のバイナリを返却
	return shaderBlob;
}

void SiEngine::InitializeDxcCompiler()
{
	HRESULT hr;
	dxcUtils_ = nullptr;
	dxcCompiler_ = nullptr;

	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(hr));

	//現時点でincludeはしないが、includeに対応するための設定を行っていく
	includeHandler_ = nullptr;
	hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr));
}

void SiEngine::CreateRootSignature()
{
	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//RootParameter作成、複数設定可能な為、配列に
	D3D12_ROOT_PARAMETER rootParameters[3] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;//レジスタ番号0とバインド
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;//レジスタ番号0とバインド
	descriptionRootSignature.pParameters = rootParameters;//ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);//配列の長さ

	//DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers[0].ShaderRegister = 0;
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);
	//シリアライズしてバイナリにする
	signatureBlob_ = nullptr;
	errorBlob_ = nullptr;
	HRESULT hr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);

	if (FAILED(dxCommon_->GetHr()))
	{
		Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
		assert(false);
	}

	//バイナリを元に生成
	rootSignature_ = nullptr;
	hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
		signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(hr));
}

void SiEngine::CreateInputlayOut()
{
	inputElementDescs_[0].SemanticName = "POSITION";
	inputElementDescs_[0].SemanticIndex = 0;
	inputElementDescs_[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs_[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs_[1].SemanticName = "TEXCOORD";
	inputElementDescs_[1].SemanticIndex = 0;
	inputElementDescs_[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs_[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputLayoutDesc_.pInputElementDescs = inputElementDescs_;
	inputLayoutDesc_.NumElements = _countof(inputElementDescs_);
}

void SiEngine::BlendState()
{
	//すべての色要素を書き込む
	blendDesc_.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc_.RenderTarget[0].BlendEnable = TRUE;
	blendDesc_.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc_.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc_.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc_.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc_.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
}

void SiEngine::RasterizerState()
{
	//裏面（時計回り）を表示しない
	rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;

	//三角形の中を塗りつぶす
	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;

	//Shaderをコンパイルする
	vertexShaderBlob_ = CompileShader(L"Object3d.VS.hlsl",
		L"vs_6_0", dxcUtils_, dxcCompiler_, includeHandler_);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = CompileShader(L"Object3d.PS.hlsl",
		L"ps_6_0", dxcUtils_, dxcCompiler_, includeHandler_);
	assert(pixelShaderBlob_ != nullptr);
}

void SiEngine::InitializePSO()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	//RootSignature
	graphicsPipelineStateDesc.pRootSignature = rootSignature_;

	//Inputlayout
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc_;

	//vertexShader
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(),
		vertexShaderBlob_->GetBufferSize() };

	//pixcelShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(),
		pixelShaderBlob_->GetBufferSize() };

	//BlendState
	graphicsPipelineStateDesc.BlendState = blendDesc_;

	//rasterizerState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc_;

	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//どのように画面に色を打ち込むのかの設定（気にしなく良い）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//実際に生成
	graphicsPipelineState_ = nullptr;
	HRESULT hr = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState_));
	assert(SUCCEEDED(hr));
}

void SiEngine::ViewPort()
{
	//クライアント領域のサイズと一緒にして画面全体に表示
	viewport_.Width = winApp_->GetkClientWidth();
	viewport_.Height = winApp_->GetkClientWidth();
	viewport_.TopLeftX = 0;
	viewport_.TopLeftY = 0;
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;
}

void SiEngine::ScissorRect()
{
	//シザー短形
	scissorRect_.left = 0;
	scissorRect_.right = winApp_->GetkClientWidth();
	scissorRect_.top = 0;
	scissorRect_.bottom = winApp_->GetkClientWidth();
}

void SiEngine::Draw() {
	for (int i = 0; i < 10; i++) {
		triangle_[i]->Draw(triangleData[i].material, worldMatrix_);
	}
	
}
void SiEngine::Variable() {

	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	//cameraTransform_ = { {1.0f, 1.0f, 1.0f}, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,-5.0f } };

	triangleData[0].v1 = { -0.6f,0.8f,0.0f,2.0f };
	triangleData[0].v2 = { -0.6f,1.8f,0.0f,2.0f };
	triangleData[0].v3 = { 0.6f,0.8f,0.0f,2.0f };
	triangleData[0].v4 = { 0.6f,1.8f,0.0f,2.0f };
	triangleData[0].material = { 1.0f,1.0f,1.0f,1.0f };

	/*triangleData[1].v1 = { -0.4f,0.7f,0.0f,2.0f };
	triangleData[1].v2 = { -0.4f,1.7f,0.0f,2.0f };
	triangleData[1].v3 = { 0.7f,0.7f,0.0f,2.0f };
	triangleData[1].v4 = { 0.7f,1.7f,0.0f,2.0f };
	triangleData[1].material = { 1.0f,1.0f,0.0f,1.0f };

	triangleData[2].v1 = { -0.2f,0.6f,0.0f,2.0f };
	triangleData[2].v2 = { -0.2f,1.6f,0.0f,2.0f };
	triangleData[2].v3 = { 0.8f,0.6f,0.0f,2.0f };
	triangleData[2].v4 = { 0.8f,1.6f,0.0f,2.0f };
	triangleData[2].material = { 1.0f,0.0f,1.0f,1.0f };*/

	/*triangleData[3].v1 = { -0.3f,0.5f,0.0f,2.0f };
	triangleData[3].v2 = { 0.3f,1.5f,0.0f,2.0f };
	triangleData[3].v3 = { 0.9f,0.5f,0.0f,2.0f };
	triangleData[3].material = { 0.0f,1.0f,0.0f,1.0f };

	triangleData[4].v1 = { -0.2f,0.4f,0.0f,2.0f };
	triangleData[4].v2 = { 0.4f,1.4f,0.0f,2.0f };
	triangleData[4].v3 = { 1.0f,0.4f,0.0f,2.0f };
	triangleData[4].material = { 0.0f,0.0f,0.0f,1.0f };

	triangleData[5].v1 = { -0.1f,0.3f,0.0f,2.0f };
	triangleData[5].v2 = { 0.5f,1.3f,0.0f,2.0f };
	triangleData[5].v3 = { 1.1f,0.3f,0.0f,2.0f };
	triangleData[5].material = { 1.0f,1.0f,1.0f,1.0f };*/

	


	for (int i = 0; i < 10; i++) {
		triangle_[i] = new CreateTriangle();
		triangle_[i]->Initialize(dxCommon_, triangleData[i].v1, triangleData[i].v2, triangleData[i].v3, triangleData[i].v4, triangleData[i].material,this);
	}

	/*for (int i = 0; i < 3; i++) {
		triangle_[i] = new CreateTriangle();
		triangle_[i]->Initialize(dxCommon_,triangleData1, triangleData2, triangleData3,material );
	}*/
	
}
void SiEngine::Finalize()
{
	for (int i = 0; i < 10; i++)
	{
		triangle_[i]->Finalize();
	}
	graphicsPipelineState_->Release();
	signatureBlob_->Release();
	if (errorBlob_)
	{
		errorBlob_->Release();
	}
	rootSignature_->Release();
	pixelShaderBlob_->Release();
	vertexShaderBlob_->Release();
	textureResource_->Release();
	dxCommon_->Finalize();
}
void SiEngine::BeginFrame()
{
	triangleCount_ = 0;
	dxCommon_->PreDraw();
	//viewportを設定
	dxCommon_->GetCommandList()->RSSetViewports(1, &viewport_);
	//scirssorを設定
	dxCommon_->GetCommandList()->RSSetScissorRects(1, &scissorRect_);
	//RootSignatureを設定。PS0とは別途設定が必要
	dxCommon_->GetCommandList()->SetGraphicsRootSignature(rootSignature_);
	//PS0を設定
	dxCommon_->GetCommandList()->SetPipelineState(graphicsPipelineState_);

	//dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_);

	ImGui::ShowDemoWindow();
	
}
void SiEngine::Update() {

	ImGui::Begin("Window");
	ImGui::DragFloat4("Color", &triangleData[0].material.x, 0.01f, triangleData[0].material.y,0.01f);
	ImGui::DragFloat4("transform_rotate_X", &transform_.rotate.y, 0.01f);
	ImGui::DragFloat4("transform_translate", &transform_.translate.x,0.01f);
//	ImGui::DragFloat4("transform_translate_Y", &transform_.translate.y, 0.01f);
	ImGui::End();

	transform_.rotate.y+= 0.03f;
	worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	/*Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(winApp_->GetkClientWidth())/ float(winApp_->GetkClientWidth()), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix= Multiply(worldMatrix_, Multiply(viewMatrix, projectionMatrix));

	*transformationMatrixData_ = worldViewProjectionMatrix;*/
	//worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
}

void SiEngine::EndFrame()
{
	
	ImGui::Render();
	
	dxCommon_->PostDraw();
}

void SiEngine::DrawTriangle(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d,const Vector4& material)
{
	triangleCount_++;
		triangle_[triangleCount_]->Draw(material, worldMatrix_);
	
}
DirectX::ScratchImage SiEngine::LoadTexture(const std::string& filePath)
{
	DirectX::ScratchImage mipImages = OpenImage(filePath);
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	textureResource_ = CreateTextureResource(dxCommon_->GetDevice(), metadata);
	UploadTexturData(textureResource_, mipImages);

	//metaDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	//SRVを作成するDescriptorHeapの場所を決める
	textureSrvHandleCPU_ = dxCommon_->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	textureSrvHandleGPU_ = dxCommon_->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();

	//先頭はImGuiが使っているのでその次を使う
	textureSrvHandleCPU_.ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU_.ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//SRVの作成
	dxCommon_->GetDevice()->CreateShaderResourceView(textureResource_, &srvDesc, textureSrvHandleCPU_);

	return mipImages;
}

DirectX::ScratchImage SiEngine::OpenImage(const std::string& filePath)
{
	//テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	//ミップマップの生成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	//ミップマップ付きのデータを返す
	return mipImages;
}

ID3D12Resource* SiEngine::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata)
{
	//metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);
	resourceDesc.Height = UINT(metadata.height);
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);
	resourceDesc.Format = metadata.format;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);

	//利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//Resourceの生成
	ID3D12Resource* resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));

	return resource;
}

void SiEngine::UploadTexturData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages)
{
	//Meta情報を取得
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();

	//全MipMapについて
	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; ++mipLevel)
	{
		//MipMapLevelを指定して各Imageを取得
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);

		//Textureに転送
		HRESULT hr = texture->WriteToSubresource(UINT(mipLevel), nullptr, img->pixels, UINT(img->rowPitch), UINT(img->slicePitch));
		assert(SUCCEEDED(hr));
	}
}
