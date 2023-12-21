#include "CreateTriangle.h"
#include <assert.h>
#include "SiEngine.h"

uint16_t CreateTriangle::indices[]{
	0,1,2,
	1,3,2,

};

void CreateTriangle::Initialize(DirectXCommon* dxCommon, const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d, const Vector4& material) {
	dxCommon_ = dxCommon;
	SettingVertex(a, b, c, d);
	SetResource();
}

void CreateTriangle::Draw(const Vector4& material) {
	*materialData_ = material;
	//インデックバッファビューの設定コマンド
	dxCommon_->GetCommandList()->IASetIndexBuffer(&ibView);
	//VBVを設定
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PS0に設定しているものとはまた別。同じものを設定する
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	///マテリアルCBufferの場所を指定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//描画4角形
	//dxCommon_->GetCommandList()->DrawInstanced(_countof(indices), 1, 0, 0);
	dxCommon_->GetCommandList()->DrawIndexedInstanced(_countof(indices), 10, 0, 0, 0);
}

void CreateTriangle::Finalize() {

	vertexResource_->Release();
	indexResource_->Release();
	materialResource_->Release();
}

void CreateTriangle::SettingVertex(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d) {
	vertexResource_ = CreateBufferResource(dxCommon_->GetDevice(), sizeof(Vector4) * 6);
	indexResource_ = CreateIndexResource(dxCommon_->GetDevice(), sizeof(Vector4) * 6);

	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(Vector4) * 6;
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(Vector4);

	ibView.BufferLocation = indexResource_->GetGPUVirtualAddress();

	ibView.Format = DXGI_FORMAT_R16_UINT;

	ibView.SizeInBytes = sizeIB;
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	//indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	//左下
	vertexData_[0] = a;
	//左上
	vertexData_[1] = b;
	//右上
	vertexData_[2] = c;
	//右下
	vertexData_[3] = d;
}
void CreateTriangle::SetResource() {
	materialResource_ = CreateBufferResource(dxCommon_->GetDevice(), sizeof(Vector4) * 6);
	//materialResource_ = CreateIndexResource(dxCommon_->GetDevice(), sizeof(Vector4) * 6);
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

}


ID3D12Resource* CreateTriangle::CreateBufferResource(ID3D12Device* device, size_t sizeInBytes) {
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
ID3D12Resource* CreateTriangle::CreateIndexResource(ID3D12Device* device, size_t sizeInBytes) {


	//インデックスリソース。テクスチャの場合はまた別の設定をする
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResourceDesc.Width = sizeIB;//リソースサイズ
	//インデックスの場合はこれらは１にする決まり
	ResourceDesc.Height = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.SampleDesc.Count = 1;
	//インデックスの場合はこれにする決まり
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	HRESULT hr;


	ID3D12Resource* indexBuff = nullptr;
	//実際に頂点リソースを作る
	hr = device->CreateCommittedResource(&uplodeHeapProperties, D3D12_HEAP_FLAG_NONE,
		&ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&indexBuff));
	assert(SUCCEEDED(hr));


	uint16_t* indexMap = nullptr;
	hr = indexBuff->Map(0, nullptr, (void**)&indexMap);
	for (int i = 0; i < _countof(indices); i++) {
		indexMap[i] = indices[i];
	}
	indexBuff->Unmap(0, nullptr);
	//

	return indexBuff;

}