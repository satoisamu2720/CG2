#pragma once
#include <Windows.h>
#include <cstdint>

class WinApp {
public:
   

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    static void WindowView(int32_t kClientAreaWidth, int32_t kClientAreaHeight);

    static int ProccessMessage();
    HWND GetHwnd() const { return hwnd_; }
     int32_t GetkClientAreaWidth() const { return kClientAreaWidth_; }
     int32_t GetkClientAreaHeight() const { return kClientAreaHeight_; }

private:
    //クライアント領域サイズ

    static inline WNDCLASS wc{};// ウィンドウクラス
    static HWND hwnd_;
     int kClientAreaWidth_;
     int kClientAreaHeight_;
   
   
};
