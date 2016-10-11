#pragma once

#include "precompiled.h"

class CellWindow
{
public:
	CellWindow();

	void Create(HWND parentHandle);

	void Show(int cmdShow);

	HWND getHandle() const;

	unsigned int getHeight() const;
	bool changeHeight(unsigned int countOfStrings);

	std::wstring getText() const;
	
	friend bool operator== (const CellWindow& left, const CellWindow& right);

private:

	HWND handleCellWindow;
	unsigned int height;
	unsigned int countOfStrings;

	void init();
};
