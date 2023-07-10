#pragma once
#include <Windows.h>
#include <cstdint>
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")

class WinApp {
public:
	
	

	HINSTANCE GetHInstance()const { return wc_.hInstance; }

	static	bool Procesmessage();
	static void Finalize();

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	static ID3D12Debug1* GetdebugController() { return debugController_; }

	static inline HWND GetHwnd() { return hwnd_; }
	static int32_t GetkClientWidth() { return kClientWidth_; }
	static int32_t GetkClientHeight() { return kClientHeight_; }

	static void CreateWindowView(const wchar_t* title, int32_t kClientWidth_, int32_t kClienHeight_);

private:
	static	UINT windowStyle_;

	static ID3D12Debug1* debugController_;

	static	inline 	RECT wrc_ = { 0,0,GetkClientWidth(),GetkClientHeight() };

	static inline	WNDCLASS wc_{};// ウィンドウクラス

	static const int32_t kClientWidth_ = 1280;
	static const int32_t kClientHeight_ = 720;

	static	HWND hwnd_;
};