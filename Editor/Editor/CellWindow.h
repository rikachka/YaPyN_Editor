#pragma once

#include "precompiled.h"

class CellWindow
{
public:
	CellWindow();

	void Create(HWND parentHandle);

	void Show(int cmdShow);

	HWND getHandle();

	/* На будущее.
	getData();
	*/

private:

	HWND handle;

	void init();
};
