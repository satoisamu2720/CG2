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