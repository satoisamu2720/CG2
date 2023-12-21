#pragma once
#include "DirectXCommon.h"
#include <dxcapi.h>
#include"Vector4.h"
#include "CreateTriangle.h"
#include "Sprite.h"
#pragma comment(lib,"dxcompiler.lib")

class SiEngine
{
public:
	//頂点リソースにデータを書き込む
	Vector4* vertexData_;

	IDxcBlob* CompileShader(
		//CompileShaderするShaderファイルへのパス
		const std::wstring& filePath,
		//Compielerに使用するProfile
		const wchar_t* profile,
		//初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);
	void InitializeDxcCompiler();
	void CreateRootSignature();
	void CreateInputlayOut();
	void BlendState();
	void RasterizerState();
	void InitializePSO();
	void ViewPort();
	void ScissorRect();
	void Initialize(WinApp* win, const wchar_t* title, int32_t width, int32_t height);
	void Finalize();
	void BeginFrame();
	void EndFrame();
	void Variable();
	void Update();
	void Draw();
	void DrawTriangle(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d, const Vector4& material);

private:
	static WinApp* win_;
	static	DirectXCommon* dxCommon_;
	CreateTriangle* triangle_[12];
	Sprite* sprite_[12];
	int triangleCount_;
	int spriteCount_;
	IDxcUtils* dxcUtils_;
	IDxcCompiler3* dxcCompiler_;
	IDxcIncludeHandler* includeHandler_;
	ID3DBlob* signatureBlob_;
	ID3DBlob* errorBlob_;
	ID3D12RootSignature* rootSignature_;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{};
	D3D12_BLEND_DESC blendDesc_{};
	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	D3D12_RASTERIZER_DESC rasterizerDesc_{};
	ID3D12PipelineState* graphicsPipelineState_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_VIEWPORT viewport_{};
	D3D12_RECT scissorRect_{};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[1];
	Triangle triangleData[10] = {};
	Quadrilateral quadrilateralData[10] = {};

	Vector4 material;

};