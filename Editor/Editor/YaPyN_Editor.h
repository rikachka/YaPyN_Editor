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
	void OnPaint();
	void OnSize();
	void OnDestroy();
	bool OnClose();
	void OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void OnCellClick();

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

	std::vector<HBITMAP> buttonsBitmaps;

	bool saveFile();
	bool saveFile(std::string filename);
	void loadFile(std::string filename);
	bool askToSave();

	void createCell();
	void createCell(std::wstring text);
	void deleteCell();
	void moveUpCell();
	void moveDownCell();
	void resizeCell(HWND handleCell);
	unsigned int getCountsOfStrings(HWND handleCell);
	wchar_t* getTextFromCell(HWND handleCell);

	void addToolbarItem(INT_PTR idBitmap, INT_PTR idAction);

	static LRESULT __stdcall windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

