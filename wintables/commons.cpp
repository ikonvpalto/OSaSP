#include "commons.h"

std::wstring kvp::getErrorStringRepresentation(int errorCode) {
	auto error_char_array = new wchar_t[256]{ 0 };
	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		static_cast<DWORD>(errorCode),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		error_char_array,
		0,
		nullptr);
	std::wstring error_string(error_char_array);
	delete[] error_char_array;
	return error_string;
}

void kvp::exitWithErrorMessage(const std::wstring& functionName) {
	exit(kvp::showErrorMessge(functionName));
}

//return error code
int kvp::showErrorMessge(const std::wstring& functionName) {
	auto errorCode = static_cast<int>(GetLastError());
	std::wstring errorFullMessage = functionName
		+ L" failed with error "
		+ std::to_wstring(errorCode)
		+ L": "
		+ kvp::getErrorStringRepresentation(errorCode);
	MessageBox(nullptr, errorFullMessage.c_str(), L"Error", MB_OK);
	errorFullMessage.clear();
	return errorCode;
}
