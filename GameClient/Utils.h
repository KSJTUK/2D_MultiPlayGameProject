#pragma once

inline std::wstring to_wstring(std::string_view str)
{
	return std::wstring{ str.begin(), str.end() };
}

inline void CheckHR(const HRESULT& hr, const std::source_location& location=std::source_location::current())
{
	if (FAILED(hr)) {
		_com_error err{ hr };
		std::wstring result = std::format(L"오류 위치 : {}\n{} \n\n Error : \n{}",
			to_wstring(location.file_name()), to_wstring(location.function_name()), err.ErrorMessage());

		::MessageBoxW(nullptr, result.c_str(), L"Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(EXIT_FAILURE);
	}
}

inline std::tuple<BYTE, BYTE, BYTE> GetRGB(const D2D1::ColorF::Enum& colorEnum)
{
	BYTE r = static_cast<BYTE>(colorEnum >> 16);
	BYTE g = static_cast<BYTE>(colorEnum >> 8);
	BYTE b = static_cast<BYTE>(colorEnum);
	return std::make_tuple(r, g, b);
}

inline unsigned long CreateColorFromRGB(BYTE r, BYTE g, BYTE b)
{
	unsigned long color{ };
	color |= static_cast<unsigned long>(r) << 16;
	color |= static_cast<unsigned long>(g) << 8;
	color |= static_cast<unsigned long>(b);
	return color;
}

inline D2D1::ColorF Color(const D2D1::ColorF::Enum& colorEnum, float alpha=1.0f)
{
	return D2D1::ColorF{ colorEnum, alpha };
}

inline D2D1::ColorF Color(unsigned long colorRGB, float alpha = 1.0f)
{
	return D2D1::ColorF{ colorRGB, alpha };
}

inline D2D1::ColorF ComplementaryColor(const D2D1::ColorF::Enum& colorEnum, float alpha = 1.0f)
{
	BYTE byteMax{ 0xFF };
	auto [r, g, b] = GetRGB(colorEnum);
	return Color(CreateColorFromRGB(byteMax - r, byteMax - g, byteMax - b), alpha);
}

inline unsigned long ComplementaryColorRGB(const D2D1::ColorF::Enum& colorEnum, float alpha = 1.0f)
{
	BYTE byteMax{ 0xFF };
	auto [r, g, b] = GetRGB(colorEnum);
	return CreateColorFromRGB(byteMax - r, byteMax - g, byteMax - b);
}

inline D2D1_POINT_2F operator/(const D2D1_POINT_2F& p, float scalar)
{
	return D2D1::Point2F(p.x / scalar, p.y / scalar);
}

inline D2D1_POINT_2F operator*(const D2D1_POINT_2F& p, float scalar)
{
	return D2D1::Point2F(p.x * scalar, p.y * scalar);
}

inline D2D1_SIZE_F operator*(const D2D1_SIZE_F& size, float scalar)
{
	return D2D1::SizeF(size.width * scalar, size.height * scalar);
}

inline D2D1_SIZE_F operator*(float scalar, const D2D1_SIZE_F& size)
{
	return D2D1::SizeF(size.width * scalar, size.height * scalar);
}

inline Position SizeFToPosition(const D2D1_SIZE_F& size)
{
	return Position{ size.width, size.height };
}

inline D2D1_RECT_F CreateRectF(const Position& pos, const SizeF& size)
{
	const SizeF halfSize{ size.width * 0.5f, size.height * 0.5f };
	return D2D1::RectF(pos.x - halfSize.width, pos.y - halfSize.height, pos.x + halfSize.width, pos.y + halfSize.height);
}

inline D2D1_RECT_F CreateRectF(const Position& pos, const Size& size)
{
	const SizeF halfSize{ static_cast<float>(size.width) * 0.5f, static_cast<float>(size.height) * 0.5f };
	return D2D1::RectF(pos.x - halfSize.width, pos.y - halfSize.height, pos.x + halfSize.width, pos.y + halfSize.height);
}

/* 자주 쓸만한 색상의 solid brush들을 모아놓자 */
class SolidBrush {
public:
	static void Init(ComPtr<ID2D1HwndRenderTarget> renderTarget) 
	{
		SolidBrush::renderTarget = renderTarget;
	}

	static void Destroy()
	{
		brushMap.clear();
	}

	static ID2D1SolidColorBrush* GetBrush(D2D1::ColorF::Enum color)
	{
		unsigned long c = static_cast<unsigned long>(color);
		if (brushMap.contains(c)) {
			return brushMap[c].Get();
		}

		ComPtr<ID2D1SolidColorBrush> brush{ nullptr };
		renderTarget->CreateSolidColorBrush(Color(color), brush.GetAddressOf());
		brushMap[c] = brush;
		return brushMap[c].Get();
	}

	static ID2D1SolidColorBrush* GetBrush(unsigned long color)
	{
		if (brushMap.contains(color)) {
			return brushMap[color].Get();
		}

		ComPtr<ID2D1SolidColorBrush> brush{ nullptr };
		renderTarget->CreateSolidColorBrush(Color(color), brush.GetAddressOf());
		brushMap[color] = brush;
		return brushMap[color].Get();
	}

private:
	inline static ComPtr<ID2D1HwndRenderTarget> renderTarget{ nullptr };
	inline static std::unordered_map<unsigned long, ComPtr<ID2D1SolidColorBrush>> brushMap;
};