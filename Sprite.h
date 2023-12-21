#pragma once
#include"DirectXCommon.h"
#include"Vector4.h"
#include"Vertex.h"
#include"Matrix.h"
#include "Matrix4x4.h"
#include "WinApp.h"
	typedef struct Quadrilateral {
	Vector4 v1;
	Vector4 v2;
	Vector4 v3;
	Vector4 v4;
	Vector4 material;
};
class SpriteEngine;

class Sprite {
public:
	void Initialize(DirectXCommon* dxCommon, const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d, const Vector4& material);
	void Draw(const Vector4& material);
	void Finalize();
private:
	void SettingVertex(const Vector4& a, const Vector4& b, const Vector4& c ,const Vector4&d);
	void SetResource();

	WinApp* winApp_;
	SpriteEngine* Engine_;
	DirectXCommon* dxCommon_;
	VertexData* vertexData_ = nullptr;
	Vector4* materialData_;
	Matrix4x4* transformationMatrixDataSprite_ = nullptr;
	ID3D12Resource* vertexResourceSprite_;
	ID3D12Resource* materialResourceSprite_;
	ID3D12Resource* transformationMatrixResourceSprite_;
	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite_{};
	int i;
	Transform transformSprite{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
};


