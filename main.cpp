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
			engin->BeginFrame();
		
			engin->DrawTriangle(triangle1Data, triangle2Data, triangle3Data,material1Data);

			engin->DrawTriangle(triangle4Data, triangle5Data, triangle6Data, material2Data);

			engin->DrawTriangle(triangle7Data, triangle8Data, triangle9Data, material3Data);


			engin->EndFrame();

		}
	}
	OutputDebugStringA("Hello,DirectX!\n");
	engin->Finalize();
	return 0;
}