#pragma once

std::wstring to_wstring(std::string_view str)
{
	return std::wstring{ str.begin(), str.end() };
}

void CheckHR(const HRESULT& hr, const std::source_location& location)
{
	if (FAILED(hr)) {
		_com_error err{ hr };
		std::wstring result = std::format(L"오류 위치 : {}\n{} \n\n Error : \n{}",
			to_wstring(location.file_name()), to_wstring(location.function_name()), err.ErrorMessage());

		::MessageBoxW(nullptr, result.c_str(), L"Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(EXIT_FAILURE);
	}
}