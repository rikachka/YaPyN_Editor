﻿#pragma once

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
	void OnSize();
	void OnDestroy();
	bool OnClose();
	void OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

	HWND handleMainWindow;
	HWND hWndToolBar;
	std::list<CellWindow> childrensWindow;
	std::map<HWND, CellWindow*> cellsAndHandles;

	static const int sizeBetweenCells;
	static const int marginLeftRightCells;

	bool changed;

	HBRUSH activeBrush;

	void saveFile(std::string pathToFile);
	void loadFile(std::string pathToFile);
	void createToolbar();

	void createCell();
	void deleteCell();
	void resizeCell(HWND handleCell);
	unsigned int getCountsOfStrings(HWND handleCell);

	static LRESULT __stdcall windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

