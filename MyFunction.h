#pragma once
#include <string>
#include "DIrectXCommon.h"
#include <dxgidebug.h>
#pragma comment(lib,"dxguid.lib")

void Log(const std::string& message);

 std::wstring ConvertString(const std::string& str);

 std::string ConvertString(const std::wstring& str);

