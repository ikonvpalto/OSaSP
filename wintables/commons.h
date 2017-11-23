#pragma once

#ifndef UNICODE
#define UNICODE
#endif //UNICODE

#ifndef _UNICODE
#define _UNICODE
#endif //_UNICODE

#include <windows.h>
#include <string>

namespace kvp {

	int showErrorMessge(const std::wstring &functionName);
	void exitWithErrorMessage(const std::wstring &functionName);
	std::wstring getErrorStringRepresentation(int errorCode);

}