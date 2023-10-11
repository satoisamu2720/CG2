#pragma once
#include"DirectXCommon.h"
#include"Vector4.h"
typedef struct Triangle {
	Vector4 v1;
	Vector4 v2;
	Vector4 v3;
	Vector4 v4;
	Vector4 material;
};
class CreateEngine;

class CreateTriangle {
public:
	void Initialize(DirectXCommon* dxCommon, const Vector4& a, const Vector4& b, const Vector4& c,const Vector4& d, const Vector4& material);
	void Draw(const Vector4& material);
	void Finalize();
private:
	void SettingVertex(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d);
	void SetResource();
	
	static uint16_t indices[6];
	CreateEngine* Engine_;
	DirectXCommon* dxCommon_;
	D3D12_HEAP_PROPERTIES uplodeHeapProperties{};
	Vector4* vertexData_;
	Vector4* materialData_;
	D3D12_RESOURCE_DESC ResourceDesc{};
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));
	ID3D12Resource* vertexResource_;
	ID3D12Resource* materialResource_;
	ID3D12Resource* indexResource_;
	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);
	ID3D12Resource* CreateIndexResource(ID3D12Device* device, size_t sizeInBytes);
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	int i;
};