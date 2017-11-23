#pragma once

#include "commons.h"
#include "TableCell.h"

class Table {
private :
	TableCell **cells;
	int width;
	int height;
	int rows;
	int columns;
	TableCell *activeCell;
public:

	Table(int width, int height, int rows, int columns);

	int getWidth() const;
	void setWidth(int width);
	int getHeight() const;
	void setHeight(int height);
	const POINT& getSize() const;
	void setSize(const POINT &size);
	int getRows() const;
	int getColumns() const;
	TableCell& getCell(int row, int column);
	void activateCell(int row, int column);
	void deactivateCell();
	bool isAnyCellActive();

	
	~Table();
};

