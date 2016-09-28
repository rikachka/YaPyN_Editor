#pragma once

#include "CellWindow.h"
#include "precompiled.h"

class YaPyN_Editor
{
public:
	YaPyN_Editor();
	~YaPyN_Editor();
		
	static bool RegisterClass();

	bool Create();

	void Show(int cmdShow);

protected:

	void OnNCCreate(HWND hwnd);
	void OnCreate();
	void OnSize(WPARAM wParam);
	void OnDestroy();
	bool OnClose();
	void OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

	HWND handle;
	HWND hWndToolBar;
	std::list<CellWindow> childrensWindow;

	bool changed;

	HBRUSH activeBrush;

	void saveFile(std::string pathToFile);
	void loadFile(std::string pathToFile);
	void CreateToolbar();

	LRESULT OnControlColorEdit(HDC hdc);

	static LRESULT __stdcall windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

