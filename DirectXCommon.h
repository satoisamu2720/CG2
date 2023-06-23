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
	static void Initialize();
	static void ChooseDXGIFactory();
	static void ChooseAdapter();
	static void CreateDevice();
	static void CreateCommand();
	static void CreateSwapChain();
	static void CreateFinalRenderTargets();
	static void CreateFence();
	static void Release();
	static void ResourceCheck();


private: // メンバ変数
	static HWND hwnd_;
	static ID3D12Debug1* debugController;
	static IDXGIFactory7* dxgifactory;
	static IDXGIAdapter4* useAdapter;
	static ID3D12Device* device;
	static ID3D12InfoQueue* infoQueue;
	static ID3D12Resource* swapChainResources[2];
	static ID3D12CommandQueue* commandQueue;
	static inline D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	static ID3D12CommandAllocator* commandAllocator;
	static ID3D12GraphicsCommandList* commandList;
	static IDXGISwapChain4* swapChain;
	static inline DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	static ID3D12DescriptorHeap* rtvDescriptorHeap;
	static inline D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
	static uint64_t fenceValue;
	static HANDLE fenceEvent;
	static ID3D12Fence* fence;
	static WinApp* winApp_;
	static IDXGIDebug1*debug;
	static int32_t kClientAreaWidth_;
	static int32_t kClientAreaHeight_;
	static HRESULT hr;
};

