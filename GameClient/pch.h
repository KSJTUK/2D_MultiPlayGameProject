#pragma once

#include "targetver.h"

#include <Windows.h>

/* Direct2D Library */
#include <d2d1_1.h>                 /* Direct2D API */
#include <d2d1_1helper.h>           /* C++ ���� ������ �Լ�, Ŭ����, ����ü���� ���ǵ� */
#include <d2dbasetypes.h>           /* ��, ��, �簢�� �� �⺻���� �׸��� ���� */
#include <d2derr.h>                 /* Direct2D API ���� ����ϴ� ���� �ڵ尡 ���ǵ� ��� */
#include <wincodec.h>               /* WIC ����� ���� ��� */
#include <d2d1effects.h>            /* �̹��� ȿ���� ���õ� �͵� ���� */
#include <d2d1effecthelpers.h>      /* �̹��� ȿ�� ���� C++ ���� */
#include <dwrite_3.h>               /* �ؽ�Ʈ ����� ���� ��� */

/* Com ��ü */
#include <comdef.h>
#include <wrl/client.h>

#pragma comment(lib, "DWrite.lib")
#pragma comment(lib, "D2D1.lib")

/* debug �뵵 */
#include <crtdbg.h>
#include <source_location>

/* C++ ǥ�� */
#include <functional>
#include <array>
#include <queue>
#include <set>

/* string */
#include <string>
#include <string_view>
#include <format>

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

/* ��Ÿ */
#include "types.h"

using namespace std::literals;
using WINPROC = LRESULT(*)(HWND, UINT, WPARAM, LPARAM);