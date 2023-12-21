#include "Sprite.h"
#include <assert.h>
#include "SiEngine.h"

void Sprite::Initialize(DirectXCommon* dxCommon, const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d, const Vector4& material) {
	dxCommon_ = dxCommon;
	SettingVertex(a, b, c, d);
	SetResource();
}

void Sprite::Draw(const Vector4& material) {
	*materialData_ = material;
	//VBVを設定
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1,&vertexBufferViewSprite_);
	//形状を設定。PS0に設定しているものとはまた別。同じものを設定する
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	///マテリアルCBufferの場所を指定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSprite_->GetGPUVirtualAddress());
	//描画
	dxCommon_->GetCommandList()->DrawInstanced(6, 1, 0, 0);
}

void Sprite::Finalize() {

	vertexResourceSprite_->Release();
	materialResourceSprite_->Release();
	transformationMatrixResourceSprite_->Release();
}

void Sprite::SettingVertex(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4&d) {
	vertexResourceSprite_ = CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * 6);
	//リソースの先頭のアドレスから使う
	vertexBufferViewSprite_.BufferLocation = vertexResourceSprite_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferViewSprite_.SizeInBytes = sizeof(VertexData) * 6;
	//1頂点当たりのサイズ
	vertexBufferViewSprite_.StrideInBytes = sizeof(VertexData);
	//書き込むためのアドレスを取得
	vertexResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	//1枚目 
	vertexData_[0].position = a; //左下
	vertexData_[0].texcoord = { 0.0f,1.0f };

	vertexData_[1].position = b;//左上
	vertexData_[1].texcoord = { 0.0f,0.0f };

	vertexData_[2].position = c;//右下
	vertexData_[2].texcoord = { 1.0f,1.0f };

	//2枚目
	vertexData_[3].position = b;//左上
	vertexData_[3].texcoord = { 0.0f,0.0f };

	vertexData_[4].position = d;//右上
	vertexData_[4].texcoord = { 1.0f,0.0f };

	vertexData_[5].position = c;//右下
	vertexData_[5].texcoord = { 1.0f,1.0f };

	/*Matrix4x4 worldMatrixSprite = MakeAffineMatrix(transformSprite.scale, transformSprite.rotate, transformSprite.translate);
	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
	Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(winApp_->GetkClientWidth()), float(winApp_->GetkClientWidth()), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrixSprite = Multiply(worldMatrixSprite, Multiply(viewMatrixSprite, projectionMatrixSprite));

	*transformationMatrixDataSprite_ = worldViewProjectionMatrixSprite;*/
}
void Sprite::SetResource() {
	transformationMatrixResourceSprite_ = CreateBufferResource(dxCommon_->GetDevice(), sizeof(Matrix4x4));
	transformationMatrixResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite_));
	*transformationMatrixDataSprite_ = MakeIdentity4x4();
}


ID3D12Resource* Sprite::CreateBufferResource(ID3D12Device* device, size_t sizeInBytes) {
	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uplodeHeapProperties{};
	uplodeHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;//UploadHeapを使う
	//頂点リソースの設定
	D3D12_RESOURCE_DESC ResourceDesc{};
	//バッファリソース。テクスチャの場合はまた別の設定をする
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResourceDesc.Width = sizeInBytes;//リソースサイズ
	//バッファの場合はこれらは１にする決まり
	ResourceDesc.Height = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.SampleDesc.Count = 1;
	//バッファの場合はこれにする決まり
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	HRESULT hr;

	ID3D12Resource* Resource = nullptr;
	//実際に頂点リソースを作る
	hr = device->CreateCommittedResource(&uplodeHeapProperties, D3D12_HEAP_FLAG_NONE,
		&ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&Resource));
	assert(SUCCEEDED(hr));

	return Resource;
}
