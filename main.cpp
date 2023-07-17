#include "Engine.h"
#include "CreateTriangle.h"

const wchar_t kWindowTitle[] = { L"CG2" };



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	//初期化
	WinApp* win_ = nullptr;
	Engine* engin = new Engine;
	const int kMeshCount = 10;
	//Mesh mesh[1];
	

	//Engine* engin = new Engine;
	//DX12Common* dx12Common = new DX12Common();

	engin->Initialization(win_, kWindowTitle, 1280, 720);

	engin->Initialize();

	Vector4 triangleDeta1 = {-0.6f,0.8f,0.0f,2.0f};
	Vector4 triangleDeta2 = { 0.0f,1.8f,0.0f,2.0f };
	Vector4 triangleDeta3 = { 0.6f,0.8f,0.0f,2.0f };
	Vector4 materialDeta1 = { 1.0f,1.0f,0.0f,1.0f };
	
	Vector4 triangleDeta4 = { -1.2f,-0.2f,0.0f,2.0f};
	Vector4 triangleDeta5 = { -0.6f,0.8f,0.0f,2.0f };
	Vector4 triangleDeta6 = { 0.0f,-0.2f,0.0f,2.0f };
	Vector4 materialDeta2 = { 1.0f,1.0f,0.0f,1.0f };

	Vector4 triangleDeta7 = { 0.0f,-0.2f,0.0f,2.0f };
	Vector4 triangleDeta8 = { 0.6f,0.8f,0.0f,2.0f };
	Vector4 triangleDeta9 = { 1.2f,-0.2f,0.0f,2.0f };
	Vector4 materialDeta3 = { 1.0f,1.0f,0.0f,1.0f };

	MSG msg{};
	

	while (msg.message != WM_QUIT) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);


		}
		else {
			engin->BeginFrame();
			
			engin->DrawTriangle(triangleDeta1, triangleDeta2, triangleDeta3, materialDeta1);

			engin->DrawTriangle(triangleDeta4, triangleDeta5, triangleDeta6, materialDeta2);

			engin->DrawTriangle(triangleDeta7, triangleDeta7, triangleDeta9, materialDeta3);


			engin->EndFrame();

		}
	}
	OutputDebugStringA("Hello,DirectX!\n");
	engin->Finalize();
	return 0;
}