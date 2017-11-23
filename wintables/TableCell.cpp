#include "TableCell.h"

TableCell::TableCell(const std::wstring & text) {
	this->text = text;
}

const std::wstring & TableCell::getText() const {
	return text;
}

void TableCell::appendText(const std::wstring & text) {
	this->text.append(text);
}

void TableCell::deleteLastChar() {
	text.erase(text.size() - 1);
}

void TableCell::clear() {
	text.clear();
}

TableCell::~TableCell() {
	clear();
}
