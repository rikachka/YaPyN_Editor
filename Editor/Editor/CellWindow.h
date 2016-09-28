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

private:

	HWND handle;
	unsigned int height;

	void init();
};
