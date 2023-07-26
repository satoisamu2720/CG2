#include "SiEngine.h"
#include "CreateTriangle.h"

const wchar_t kWindowTitle[] = { L"CG2" };



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	//初期化

	WinApp* win_ = nullptr;
	SiEngine* engin_ = new SiEngine;

	engin_->Initialization(win_, kWindowTitle, 1280, 720);

	engin_->Initialize();

	Vector4 triangle1Data= {-0.6f,0.8f,0.0f,2.0f};
	Vector4 triangle2Data = { 0.0f,1.8f,0.0f,2.0f };
	Vector4 triangle3Data= { 0.6f,0.8f,0.0f,2.0f };
	Vector4 material1Data = { 1.0f,0.0f,0.0f,1.0f };
	
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
		//一つ目
			engin_->DrawTriangle(triangle1Data, triangle2Data, triangle3Data,material1Data);
		//二つ目
			engin_->DrawTriangle(triangle4Data, triangle5Data, triangle6Data, material2Data);
		//三つ目
			engin_->DrawTriangle(triangle7Data, triangle8Data, triangle9Data, material3Data);

			engin_->EndFrame();
		}
	}
	OutputDebugStringA("Hello,DirectX!\n");
	engin_->Finalize();
	return 0;
}