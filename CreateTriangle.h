#pragma once
#include"DirectXCommon.h"
#include"Vector4.h"
typedef struct Triangle {
	Vector4 v1;
	Vector4 v2;
	Vector4 v3;
};
class CreateEngine;

class CreateTriangle {
public:
	void Initialize(DirectXCommon* dxCommon, const Vector4& a, const Vector4& b, const Vector4& c);
	void Draw();
	void Finalize();
private:
	void SettingVertex(const Vector4& a, const Vector4& b, const Vector4& c);
	
	

	CreateEngine* Engine_;
	DirectXCommon* dxCommon_;
	Vector4* vertexData_;
	ID3D12Resource* vertexResource_;
	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	int i;
};