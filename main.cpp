#include "SiEngine.h"
#include "CreateTriangle.h"

const wchar_t kWindowTitle[] = { L"CG2" };



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	CoInitializeEx(0, COINIT_MULTITHREADED);
	//初期化

	WinApp* win_ = nullptr;
	SiEngine* engine_ = new SiEngine;
	DirectXCommon* dxCommon_ = new DirectXCommon;
	engine_->Initialize(win_, kWindowTitle, 1280, 720);
	engine_->LoadTexture("resources/uvChecker.png");
	engine_->Variable();

	MSG msg{};
	

	while (msg.message != WM_QUIT) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);


		}
		else {
			engine_->BeginFrame();
			engine_->Draw();
			engine_->Update();
			engine_->EndFrame();
		}
	}
	OutputDebugStringA("Hello,DirectX!\n");
	engine_->Finalize();

	CoUninitialize();
	return 0;
}