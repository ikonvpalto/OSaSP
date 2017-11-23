#pragma once

#include "commons.h"
#include <string>

class TableCell {
private:
	std::wstring text;
public:
	TableCell(const std::wstring &text = L"");

	const std::wstring& getText() const;
	void appendText(const std::wstring &text);
	void deleteLastChar();
	void clear();

	~TableCell();
};

