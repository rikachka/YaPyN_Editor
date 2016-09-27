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
	void OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

	HWND handle;
	bool changed;

	void saveFile(std::string pathToFile);
	void loadFile(std::string pathToFile);


	static LRESULT __stdcall windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

