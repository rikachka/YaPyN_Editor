#include "CellWindow.h"

// Временный размер окна текста.
const int MAGIC_NUMBER = 34;
// Временный размер одной строчки.
const int heightOfString = 16;
// Временный размер одного символа.
const long widthOfSymbol = 10;



bool operator== (const CellWindow& left, const CellWindow& right)
{
	return left.getHandle() == right.getHandle();
}



CellWindow::CellWindow()
{
	handleCellWindow = 0;
	height = MAGIC_NUMBER;
	countOfStrings = 0;
}

void CellWindow::Create(HWND parentHandle)
{
	checkHandle(parentHandle);

	//can use ES_AUTOHSCROLL, if we want to move inside the cell only by using the pointer
	handleCellWindow = CreateWindowEx(0,
		L"EDIT",
		0,
		WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE | WS_BORDER //| ES_AUTOHSCROLL
																	| WS_HSCROLL,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		parentHandle,
		0,
		GetModuleHandle(0),
		0);

	//TODO: Add scrollbar, when the length of the text is larger than the length of the window
	ShowScrollBar(handleCellWindow, SB_HORZ, FALSE);

	checkHandle(handleCellWindow);

	init();
	SetFocus(handleCellWindow);
}

void CellWindow::Show(int cmdShow)
{
	ShowWindow(handleCellWindow, cmdShow);
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



HWND CellWindow::getHandle() const
{
	return handleCellWindow;
}

unsigned int CellWindow::getHeight() const
{
	return height;
}

bool CellWindow::changeHeight(unsigned int newCountOfStrings)
{
	bool changed = (countOfStrings != newCountOfStrings);
	countOfStrings = newCountOfStrings;
	height = MAGIC_NUMBER + countOfStrings * heightOfString;
	// Поправить
	return changed;
}

std::wstring CellWindow::getText()
{
	int length = SendMessage(handleCellWindow, WM_GETTEXTLENGTH, 0, 0);
	length++;
	std::wstring text;
	text.resize(length);
	::GetWindowText(handleCellWindow, (LPWSTR)text.c_str(), length);
	return text;
}
