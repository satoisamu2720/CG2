#pragma once
#include"DirectXCommon.h"
#include"Vector4.h"

typedef struct Quadrilateral {
	Vector4 v1;
	Vector4 v2;
	Vector4 v3;
	Vector4 v4;
	Vector4 material;
};
class CreateQuadrilateral;

class CreateQuadrilateral{
public:
	void Initialize(DirectXCommon* dxCommon, const Vector4& a, const Vector4& b, const Vector4& c,const Vector4& d, const Vector4& material);
	void Draw(const Vector4& material);
	void Finalize();
private:
	void SettingVertex(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d);
	void SetResource();


	static uint16_t indices[6];
	Quadrilateral* Engine_;
	DirectXCommon* dxCommon_;
	D3D12_HEAP_PROPERTIES uplodeHeapProperties{};
	Vector4* vertexDataSprite_;
	Vector4* materialDataSprite_;
	D3D12_RESOURCE_DESC ResourceDesc{};
	
	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));
	ID3D12Resource* vertexResourceSprite_;
	ID3D12Resource* materialResource_;
	
	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);
	ID3D12Resource* CreateIndexResource(ID3D12Device* device, size_t sizeInBytes);
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite_;
	int i;
	ID3D12Resource* indexBuff = nullptr;


	ID3D12Resource* indexResourceSprite_;
	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite{};
	uint32_t* indexDataSprite = nullptr;

};
