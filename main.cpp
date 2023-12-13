#include "SiEngine.h"
#include "CreateTriangle.h"

const wchar_t kWindowTitle[] = { L"CG2" };


// Windowsアプリでのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	// 初期化
	WinApp* win_ = nullptr;
	SiEngine* engine = new SiEngine;
	engine->Initialization(win_, kWindowTitle, 1280, 720);

	engine->Initialize();

	engine->LoadTexture("resources/uvChecker.png");

	Vector4 leftTop = { 0.0f,0.0f,0.0f,1.0f };
	Vector4 leftBottom = { 0.0f,360.0f,0.0f,1.0f };
	Vector4 rightTop = { 640.0f,0.0f,0.0f,1.0f };
	Vector4 rightBottom = { 640.0f,360.0f,0.0f,1.0f };

	Vector4 triangleVertexData[3][3];
	Vector4 material[3] = {
		{1, 1, 1, 1},
		{1, 1, 1, 1},
		{1, 1, 1, 1}
	};
	float materialColor0[3] = { material[0].x, material[0].y, material[0].z };
	float materialColor1[3] = { material[1].x, material[1].y, material[1].z };
	float materialColor2[3] = { material[2].x, material[2].y, material[2].z };

	// 真ん中の三角形
	triangleVertexData[1][0] = { -0.4f, -0.2f, 0.0f, 1.0f };
	triangleVertexData[1][1] = { 0.0f, 0.4f, 0.0f, 1.0f };
	triangleVertexData[1][2] = { 0.4f, -0.2f, 0.0f, 1.0f };
	material[1] = { material[1].x, material[1].y, material[1].z, 1.0f };

	MSG msg{};

	while (true) {
		// windowのメッセージを最優先で処理させる
		if (win_->Procesmessage()) {
			break;
		}

		// ゲームの処理
		engine->Update();

		engine->BeginFrame();

		ImGui::Begin("Color");

		ImGui::ColorEdit3("CenterTriangleColor", materialColor1);

		ImGui::End();

		material[0].x = materialColor0[0];
		material[0].y = materialColor0[1];
		material[0].z = materialColor0[2];

		material[1].x = materialColor1[0];
		material[1].y = materialColor1[1];
		material[1].z = materialColor1[2];

		material[2].x = materialColor2[0];
		material[2].y = materialColor2[1];
		material[2].z = materialColor2[2];

		for (int i = 0; i < 3; i++) {
			// 三角形描画
			engine->DrawTriangle(
				triangleVertexData[i][0], triangleVertexData[i][1], triangleVertexData[i][2],
				material[i]);
		}

		engine->DrawSprite(leftTop, leftBottom, rightTop, rightBottom);

		engine->EndFrame();
	}

	// 解放処理
	engine->Release();

	CoUninitialize();

	return 0;
}