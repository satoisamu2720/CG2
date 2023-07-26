#pragma once
#include"DirectXCommon.h"
#include"Vector4.h"

class CreateEngine;

class CreateTriangle {
public:
	void Initialize(DirectXCommon* dxCommon);
	void Draw(const Vector4& a , const Vector4& b, const Vector4& c, const Vector4& material);
	void Finalize();
private:
	void SettingVertex();
	void SetResource();
	

	CreateEngine* Engine_;
	DirectXCommon* dxCommon_;
	Vector4* vertexData_;
	Vector4* materialData_;
	ID3D12Resource* vertexResource_;
	ID3D12Resource* materialResource_;
	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	int i;
};