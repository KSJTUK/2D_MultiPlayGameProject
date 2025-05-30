#pragma once

#include "targetver.h"

#include <Windows.h>

/* Direct2D Library */
#include <d2d1_1.h>                 /* Direct2D API */
#include <d2d1_1helper.h>           /* C++ 에서 유용한 함수, 클래스, 구조체등이 정의됨 */
#include <d2dbasetypes.h>           /* 점, 선, 사각형 등 기본적인 그리기 정보 */
#include <d2derr.h>                 /* Direct2D API 에서 사용하는 에러 코드가 정의된 헤더 */
#include <wincodec.h>               /* WIC 사용을 위한 헤더 */
#include <d2d1effects.h>            /* 이미지 효과와 관련된 것들 정의 */
#include <d2d1effecthelpers.h>      /* 이미지 효과 관련 C++ 헬퍼 */
#include <dwrite_3.h>               /* 텍스트 출력을 위한 헤더 */

/* Com 객체 */
#include <comdef.h>
#include <wrl/client.h>

#pragma comment(lib, "DWrite.lib")
#pragma comment(lib, "D2D1.lib")

/* debug 용도 */
#include <crtdbg.h>
#include <source_location>

/* C++ 표준 */
#include <random>

#include <functional>
#include <array>
#include <queue>
#include <set>

/* string */
#include <string>
#include <string_view>
#include <format>

#include <ranges>
#include <numeric>
#include <algorithm>

/* file */
#include <filesystem>

/* memory */
#include <memory>

/* imgui */
#include "External/imgui/imgui.h"
#include "External/imgui/imgui_impl_d2d.h"
#include "External/imgui/imgui_impl_win32.h"

/* 기타 */
#include "types.h"

using namespace std::literals;
using WINPROC = LRESULT(*)(HWND, UINT, WPARAM, LPARAM);

constexpr Size DEFAULT_WINDOW_SIZE{ 1366, 768 };

extern std::unique_ptr<class GameFrame> gGameFramework;