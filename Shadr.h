#pragma once
#include "MyFunction.h"
#include "DirectXCommon.h"
class Shadr
{
public: // メンバ関数
	/// <summary>
	/// 
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 
	/// </summary>
	static void DxcCompilShader();

	/// <summary>
	/// 
	/// </summary>
	static void PipelineStateObject();

	/// <summary>
	/// 
	/// </summary>
	static IDxcBlob* CompileShader(const std::wstring& filePath,
		const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);
private: // メンバ関数

	static HRESULT hr;
	static ID3DBlob* signatureBlob;
	static ID3DBlob* errorBlob;
	static IDxcBlob* shaderBlob;
	static IDxcBlob* vertexShaderBlob;
	static IDxcBlob* pixelShaderBlob;
	static IDxcUtils* dxcUtils;
	static IDxcCompiler3* dxcCompiler;
	static IDxcIncludeHandler* includeHandler;
	static ID3D12Resource* vertexResource;
	static ID3D12PipelineState* graphicsPipelineState;
	static ID3D12RootSignature* rootSignature;

};

