#include "CellWindow.h"

// Временный размер окна текста.
const int MAGIC_NUMBER = 34;
// Временный размер одной строчки.
const int heightOfString = 16;

CellWindow::CellWindow()
{
	handleCellWindow = 0;
	height = MAGIC_NUMBER;
}

void CellWindow::Create(HWND parentHandle)
{
	checkHandle(parentHandle);

	handleCellWindow = CreateWindowEx(0,
		L"EDIT",
		0,
		WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		parentHandle,
		0,
		GetModuleHandle(0),
		0);

	checkHandle(handleCellWindow);

	init();
	SetFocus(handleCellWindow);
}

void CellWindow::Show(int cmdShow)
{
	ShowWindow(handleCellWindow, cmdShow);
}

HWND CellWindow::getHandle() const
{
	return handleCellWindow;
}

unsigned int CellWindow::getHeight() const
{
	return height;
}

void CellWindow::increaseHeight()
{
	height += heightOfString;
}

bool operator== (const CellWindow& left, const CellWindow& right)
{
	return left.getHandle() == right.getHandle();
}

void CellWindow::init()
{
	HMODULE module = ::GetModuleHandle(0);
	HRSRC resourseHandle = ::FindResource(module, MAKEINTRESOURCE(IDR_TEXT1), L"TEXT");
	HGLOBAL handleData = ::LoadResource(module, resourseHandle);
	DWORD size = ::SizeofResource(module, resourseHandle);
	LPVOID data = LockResource(handleData);
	SetWindowText(handleCellWindow, reinterpret_cast<LPCWSTR>(data));
}
