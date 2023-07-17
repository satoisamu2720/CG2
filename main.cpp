#include "Engine.h"
#include "CreateTriangle.h"

const wchar_t kWindowTitle[] = { L"CG2" };



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	//初期化
	WinApp* win_ = nullptr;
	//CreateTriangle* Triangle = new CreateTriangle;
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
	Vector4 matrial1Data = { 1.0f,1.0f,1.0f,1.0f };
	
	Vector4 triangle4 = { -1.2f,-0.2f,0.0f,2.0f};
	Vector4 triangle5 = { -0.6f,0.8f,0.0f,2.0f };
	Vector4 triangle6 = { 0.0f,-0.2f,0.0f,2.0f };
	Vector4 matrial2Data = { 1.0f,1.0f,1.0f,1.0f };

	Vector4 triangle7 = { 0.0f,-0.2f,0.0f,2.0f };
	Vector4 triangle8 = { 0.6f,0.8f,0.0f,2.0f };
	Vector4 triangle9 = { 1.2f,-0.2f,0.0f,2.0f };
	Vector4 matrial3Data = { 1.0f,1.0f,1.0f,1.0f };



	/*Vector4 triangleData[10][3] = {
		{
			{},
			{},
			{}
		},
		{},
		{},
	};*/



	MSG msg{};
	/*for (int i = 0; i < 10; i++) {
		mesh[i].init(triangleData[i]);
	}*/

	while (msg.message != WM_QUIT) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);


		}
		else {
			engin->BeginFrame();
		
			engin->DrawTriangle(triangle1Data, triangle2Data, triangle3Data,matrial1Data);

			engin->DrawTriangle(triangle4, triangle5, triangle6, matrial1Data);

			engin->DrawTriangle(triangle7, triangle8, triangle9, matrial1Data);

			//for (int i = 0; i < 12; i++) {
			//	engine->DrawTriangle(mesh[i]);
			//	//mesh[i]->Draw();
			//}

			engin->EndFrame();

		}
	}
	OutputDebugStringA("Hello,DirectX!\n");
	engin->Finalize();
	return 0;
}