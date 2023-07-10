#include "Engine.h"
#include "CreateTriangle.h"

const wchar_t kWindowTitle[] = { L"CG2" };

typedef struct TRIANGLE_DATA {
	Vector4 v1;
	Vector4 v2;
	Vector4 v3;
};

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

	Vector4 triangle1 = {-0.6f,0.2f,0.0f,2.0f};
	Vector4 triangle2 = { 0.0f,1.8f,0.0f,2.0f };
	Vector4 triangle3 = { 0.6f,0.2f,0.0f,2.0f };
	
	Vector4 triangle4 = { -1.2f,-1.4f,0.0f,2.0f};
	Vector4 triangle5 = { -0.6f,0.2f,0.0f,2.0f };
	Vector4 triangle6 = { 0.0f,-1.4f,0.0f,2.0f };

	Vector4 triangle7 = { 0.0f,-1.4f,0.0f,2.0f };
	Vector4 triangle8 = { 0.6f,0.2f,0.0f,2.0f };
	Vector4 triangle9 = { 1.2f,-1.4f,0.0f,2.0f };



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
		
			engin->DrawTriangle(triangle1, triangle2, triangle3);

			engin->DrawTriangle(triangle4, triangle5, triangle6);

			engin->DrawTriangle(triangle7, triangle8, triangle9);

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