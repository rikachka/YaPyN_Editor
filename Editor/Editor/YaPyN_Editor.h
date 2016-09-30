#pragma once

#include "CellWindow.h"
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

	void createToolbar();

private:

	HWND handleMainWindow;
	HWND handleToolbar;
	std::list<CellWindow> childrensWindow;
	std::list<CellWindow>::iterator activeCell;
	std::map<HWND, std::list<CellWindow>::iterator> handlesAndCells;

	static const int sizeBetweenCells;
	static const int marginLeftRightCells;

	bool changed;

	HBRUSH activeBrush;

	bool saveFile(std::string pathToFile);
	void loadFile(std::string pathToFile);

	void createCell();
	void deleteCell();
	void resizeCell(HWND handleCell);
	unsigned int getCountsOfStrings(HWND handleCell);
	wchar_t* getTextFromCell(HWND handleCell);

	static LRESULT __stdcall windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

