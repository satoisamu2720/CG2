#include "WinApp.h"

const char kWindowTitle[] = "CG2_00_03";

//Windowsアプリでのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    //初期化
    WinApp::WindowView();

    //ウィンドウのxが押されるまでループ
    while (WinApp::ProccessMessage() == 0) {

    }

    return 0;
}