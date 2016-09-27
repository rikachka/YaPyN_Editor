#pragma once

#include "precompiled.h"

class YaPyN_Editor
{
public:
	YaPyN_Editor();
		
	static bool RegisterClass();

	bool Create();

	void Show(int cmdShow);

protected:

	void OnNCCreate(HWND hwnd);
	void OnCreate();
	void OnSize();
	void OnDestroy();
	bool OnClose();

private:

	HWND handle;
	bool changed;

	static LRESULT __stdcall windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

