#pragma once

#include "targetver.h"

#include <Windows.h>

/* Direct2D Library */
#include <d2d1_1.h>                 /* Direct2D API */
#include <d2d1_1helper.h>           /* C++ ���� ������ �Լ�, Ŭ����, ����ü���� ���ǵ� */
#include <d2dbasetypes.h>           /* ��, ��, �簢�� �� �⺻���� �׸��� ���� */
#include <d2derr.h>                 /* Direct2D API ���� ����ϴ� ���� �ڵ尡 ���ǵ� ��� */
#include <d2d1effects.h>            /* �̹��� ȿ���� ���õ� �͵� ���� */
#include <d2d1effecthelpers.h>      /* �̹��� ȿ�� ���� C++ ���� */

/* Com ��ü */
#include <comdef.h>
#include <wrl/client.h>

#pragma comment(lib, "D2D1.lib")

/* debug �뵵 */
#include <crtdbg.h>
#include <source_location>

/* C++ ǥ�� */

/* string */
#include <string>
#include <string_view>
#include <format>

/* file */
#include <filesystem>

/* memory */
#include <memory>

/* ��Ÿ */
#include "types.h"