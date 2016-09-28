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
	void increaseHeight();

	friend bool operator== (const CellWindow& left, const CellWindow& right);

private:

	HWND handleCellWindow;
	unsigned int height;

	void init();
};
