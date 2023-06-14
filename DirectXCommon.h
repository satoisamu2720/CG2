#pragma once

#include <string>
#include <format>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <dxcapi.h>
#include "WinApp.h"
#include "MyFunction.h"
#include <dxgidebug.h>

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")



class DirectXCommon
{
public: // メンバ関数
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;
	
	//static void Log(const std::string& message);
	/// <summary>
	/// 
	/// </summary>
	static void PreDraw();

	/// <summary>
	/// 
	/// </summary>
	static void DirectXInitialization();
	/// <summary>
	/// 
	/// </summary>
	static void InitializeDXGIDevice();

	/// <summary>
	/// 
	/// </summary>
	static void CreateSwapChain();

	/// <summary>
	/// 
	/// </summary>
	static void InitializeCommand();


	/// <summary>
	///
	/// </summary>
	static void Release();


	/// <summary>
	///
	/// </summary>
	static void ResourceCheck();

	/// <summary>
	/// フェンス生成
	/// </summary>
	static void CreateFence();

private: // メンバ関数

	static WinApp* winApp_;
	static ID3D12Device* device;
	static LRESULT lresult;
	static IDXGISwapChain4* swapChain;
	static ID3D12CommandQueue* commandQueue;
	static IDXGIFactory7* dxgifactory;
	static HRESULT hr;
	static ID3D12DescriptorHeap* rtvDescriptorHeap;
	static ID3D12Resource* swapChainResources[2];
	static ID3D12Fence* fence;
	static uint64_t fenceValue;
	static HANDLE fenceEvent;
	static IDXGIAdapter4* useAdapter;
	static ID3D12Debug1* debugController;
	static ID3D12GraphicsCommandList* commandList;
	static ID3D12CommandAllocator* commandAllocator;
	static IDXGIDebug1* debug;
	static inline DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	static inline D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	static inline D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
};

