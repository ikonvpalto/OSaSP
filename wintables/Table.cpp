#include "Table.h"


Table::Table(int width, int height, int rows, int columns) {
	this->width = width;
	this->height = height;
	this->rows = rows;
	this->columns = columns;
	cells = new TableCell*[rows];
	for (int i = 0; i < rows; i++) {
		cells[i] = new TableCell[columns]();
	}
}

int Table::getWidth() const
{
	return width;
}

void Table::setWidth(int width)
{
	this->width = width;
}

int Table::getHeight() const
{
	return height;
}

void Table::setHeight(int height)
{
	this->height = height;
}

const POINT & Table::getSize() const
{
	return { width, height };
}

void Table::setSize(const POINT & size)
{
	width = size.x;
	height = size.y;
}

int Table::getRows() const
{
	return rows;
}

int Table::getColumns() const
{
	return columns;
}

TableCell & Table::getCell(int row, int column)
{
	return cells[row][column];
}

void Table::activateCell(int row, int column)
{
	activeCell = cells[row] + column;
}

void Table::deactivateCell()
{
	activeCell = nullptr;
}

bool Table::isAnyCellActive()
{
	return nullptr == activeCell;
}

Table::~Table()
{
	for (int i = 0; i < rows; i++)
		delete[] cells[i];
	delete[] cells;
}


