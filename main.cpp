#include "WinApp.h"
#include "DirectXCommon.h"

const char kWindowTitle[] = "CG2_00_03";

//Windowsアプリでのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//初期化
	WinApp* winApp = new WinApp();
	
	DirectXCommon* directXCommon = new DirectXCommon();


	directXCommon->InitializeCommand(winApp);

	//App::WindowView();
	
	//DirectXCommon::Initialization();

	MSG msg{};
	// ウィンドウのｘボタンが押されるまでループ
	while (WinApp::ProccessMessage() == 0) {

		// Windowにメッセージが来ていたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
		
		}
	}
	return 0;
}
