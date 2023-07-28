#include "SiEngine.h"
#include "CreateTriangle.h"

const wchar_t kWindowTitle[] = { L"CG2" };



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	//初期化

	WinApp* win_ = nullptr;
	SiEngine* engin_ = new SiEngine;

	engin_->Initialization(win_, kWindowTitle, 1280, 720);

	engin_->Initialize();

	engin_->Variable();

	
	
	Vector4 triangle4Data = { -1.2f,-0.2f,0.0f,2.0f};
	Vector4 triangle5Data = { -0.6f,0.8f,0.0f,2.0f };
	Vector4 triangle6Data = { 0.0f,-0.2f,0.0f,2.0f };
	Vector4 material2Data = { 1.0f,0.0f,1.0f,1.0f };

	Vector4 triangle7Data = { 0.0f,-0.2f,0.0f,2.0f };
	Vector4 triangle8Data = { 0.6f,0.8f,0.0f,2.0f };
	Vector4 triangle9Data = { 1.2f,-0.2f,0.0f,2.0f };
	Vector4 material3Data = { 1.0f,1.0f,0.0f,1.0f };

	MSG msg{};
	

	while (msg.message != WM_QUIT) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);


		}
		else {
			engin_->BeginFrame();
			engin_->Draw();
			engin_->EndFrame();
		}
	}
	OutputDebugStringA("Hello,DirectX!\n");
	engin_->Finalize();
	return 0;
}