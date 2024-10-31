#pragma once

inline std::wstring to_wstring(std::string_view str)
{
	return std::wstring{ str.begin(), str.end() };
}

inline void CheckHR(const HRESULT& hr, const std::source_location& location)
{
	if (FAILED(hr)) {
		_com_error err{ hr };
		std::wstring result = std::format(L"오류 위치 : {}\n{} \n\n Error : \n{}",
			to_wstring(location.file_name()), to_wstring(location.function_name()), err.ErrorMessage());

		::MessageBoxW(nullptr, result.c_str(), L"Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(EXIT_FAILURE);
	}
}

inline D2D1::ColorF Color(const D2D1::ColorF::Enum& colorEnum, float alpha=1.0f)
{
	return D2D1::ColorF{ colorEnum, alpha };
}

inline D2D1_POINT_2F operator+(const D2D1_POINT_2F& p1, const D2D1_POINT_2F& p2)
{
	return D2D1::Point2F(p1.x + p2.x, p1.y + p2.y);
}

inline D2D1_POINT_2F operator/(const D2D1_POINT_2F& p, float scalar)
{
	return D2D1::Point2F(p.x / scalar, p.y / scalar);
}

inline D2D1_POINT_2F operator*(const D2D1_POINT_2F& p, float scalar)
{
	return D2D1::Point2F(p.x * scalar, p.y * scalar);
}

inline D2D1_POINT_2F SizeFToPosition(const D2D1_SIZE_F& size)
{
	return D2D1::Point2F(size.width, size.height);
}

/* 자주 쓸만한 색상의 solid brush들을 모아놓자 */
class SolidBrush {
public:
	static void Init(const ComPtr<ID2D1HwndRenderTarget>& renderTarget)
	{
		renderTarget->CreateSolidColorBrush(Color(D2D1::ColorF::Black), blackBrush.GetAddressOf());
	}

public:
	inline static ComPtr<ID2D1SolidColorBrush> blackBrush;
};