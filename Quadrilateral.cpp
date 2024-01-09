#include "Quadrilateral.h"
#include <assert.h>

uint16_t CreateQuadrilateral::indices[]{
	0,1,2,
	2,3,0,
};

void CreateQuadrilateral::Initialize(DirectXCommon* dxCommon, const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d, const Vector4& material) {
	dxCommon_ = dxCommon;
	SettingVertex(a, b, c, d);
	SetResource();
}

void CreateQuadrilateral::Draw(const Vector4& material) {
	*materialDataSprite_ = material;
	//インデックバッファビューの設定コマンド
	dxCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferViewSprite);
	//VBVを設定
    //dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PS0に設定しているものとはまた別。同じものを設定する
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	///マテリアルCBufferの場所を指定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//描画
	//dxCommon_->GetCommandList()->DrawInstanced(_countof(indices), 1, 0, 0);
	dxCommon_->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void CreateQuadrilateral::Finalize() {

	vertexResourceSprite_->Release();
	//indexResourceSprite_->Release();
	materialResource_->Release();
}

void CreateQuadrilateral::SettingVertex(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d) {
	vertexResourceSprite_ = CreateBufferResource(dxCommon_->GetDevice(), sizeof(Vector4) * 4);
	//indexResourceSprite_ = CreateIndexResource(dxCommon_->GetDevice(), sizeof(Vector4) * 6);

	//リソースの先頭のアドレスから使う
	vertexBufferViewSprite_.BufferLocation = vertexResourceSprite_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferViewSprite_.SizeInBytes = sizeof(Vector4) * 6;
	//1頂点当たりのサイズ
	vertexBufferViewSprite_.StrideInBytes = sizeof(Vector4);

	/*indexBufferViewSprite.BufferLocation = indexResourceSprite_->GetGPUVirtualAddress();

	indexBufferViewSprite.SizeInBytes = sizeIB;

	indexBufferViewSprite.Format = DXGI_FORMAT_R16_UINT;*/

	//書き込むためのアドレスを取得
	vertexResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite_));
	//indexResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite));

	vertexDataSprite_[0] = a;   vertexDataSprite_[1] = b;   vertexDataSprite_[2] = d;
	vertexDataSprite_[3] = b;   vertexDataSprite_[4] = c;   vertexDataSprite_[5] = d;
}

void CreateQuadrilateral::SetResource() {
	materialResource_ = CreateBufferResource(dxCommon_->GetDevice(), sizeof(Vector4) * 6);
	//materialResource_ = CreateIndexResource(dxCommon_->GetDevice(), sizeof(Vector4) * 6);
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSprite_));
}
ID3D12Resource* CreateQuadrilateral::CreateBufferResource(ID3D12Device* device, size_t sizeInBytes) {
	//頂点リソース用のヒープの設定
	uplodeHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;//UploadHeapを使う
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

ID3D12Resource* CreateQuadrilateral::CreateIndexResource(ID3D12Device* device, size_t sizeInBytes) {
	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uplodeHeapProperties{};
	uplodeHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;//UploadHeapを使う
	//頂点リソースの設定
	D3D12_RESOURCE_DESC ResourceDesc{};
	//バッファリソース。テクスチャの場合はまた別の設定をする
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResourceDesc.Width = sizeIB;//リソースサイズ
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