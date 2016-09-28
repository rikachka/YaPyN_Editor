#include "YaPyN_Editor.h"

// Временные путь для сохранения/загрузки.
const std::string filePathToLoad = "file.txt";
const std::string filePathToSave = "file.txt";
// Временный размер окна текста.
const int MAGIC_NUMBER = 100;

YaPyN_Editor::YaPyN_Editor()
{
	handle = 0;
	changed = false;
	childrensWindow.resize(0);
}

YaPyN_Editor::~YaPyN_Editor()
{
	handle = 0;
	changed = false;
	childrensWindow.resize(0);
}

bool YaPyN_Editor::RegisterClass()
{
	WNDCLASSEX windowClass;
	::ZeroMemory(&windowClass, sizeof(windowClass));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = YaPyN_Editor::windowProc;
	windowClass.hInstance = GetModuleHandle(0);
	windowClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	windowClass.lpszClassName = L"YaPyN_Editor";

	return (::RegisterClassEx(&windowClass) != 0);
}

bool YaPyN_Editor::Create()
{
	CreateWindowEx(0, 
		L"YaPyN_Editor",
		L"YaPyN Editor v1.0",
		WS_EX_OVERLAPPEDWINDOW | WS_SIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		GetModuleHandle(0),
		this);

	CreateToolbar();
	if( handle == 0 ) {
		MessageBox(handle, L"Error: CreateWindowEx return 0", NULL, MB_OK);
	}
	return (handle != 0);
}

void YaPyN_Editor::Show(int cmdShow)
{
	ShowWindow(handle, cmdShow);
	for( auto window = childrensWindow.begin(); window != childrensWindow.end(); ++window ) {
		window->Show(cmdShow);
	}
}

void YaPyN_Editor::OnNCCreate(HWND hwnd)
{
	handle = hwnd;
}

void YaPyN_Editor::OnCreate()
{
	CellWindow cell;
	childrensWindow.push_back(std::move(cell));
	childrensWindow.back().Create(handle);
}

void YaPyN_Editor::OnSize(WPARAM wParam)
{
	InvalidateRect(handle, NULL, FALSE);
	PAINTSTRUCT paintStruct;
	BeginPaint(handle, &paintStruct);

	HBRUSH brush;
	brush = CreateSolidBrush(RGB(100, 150, 200));
	FillRect(paintStruct.hdc, &paintStruct.rcPaint, brush);
	EndPaint(handle, &paintStruct);

	RECT rect;
	::GetClientRect(handle, &rect);

	SetWindowPos(childrensWindow.back().getHandle(), HWND_TOP, rect.left, rect.top, rect.right - rect.left, MAGIC_NUMBER, 0);
}

void YaPyN_Editor::OnDestroy()
{
	PostQuitMessage(SuccessDestroyWindowValue);
}

bool YaPyN_Editor::OnClose()
{
	if( changed ) {
		switch (MessageBox(handle, L"Вы ввели текст. Сохранить?", L"Завершение работы", MB_YESNOCANCEL | MB_ICONWARNING))
		{
			case IDYES:
			{
				saveFile(filePathToSave);
				return true;
			}
			case IDNO:
			{
				return true;
			}
			case IDCANCEL:
			{
				return false;
			}
		}
	} else {
		switch (MessageBox(handle, L"Вы действительно хотите выйти?", L"Завершение работы", MB_YESNO | MB_ICONWARNING)) {
			case IDNO:
			{
				return false;
			}
			case IDYES:
			{
				return true;
			}
		}
	}
	return true;
}

void YaPyN_Editor::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( HIWORD(wParam) == 0 ) {
		switch( LOWORD(wParam) ) {
			case ID_FILE_EXIT:
			{
				SendMessage(hWnd, WM_CLOSE, wParam, lParam);
				break;
			}
			case ID_FILE_SAVE:
			{
				saveFile(filePathToSave);
				break;
			}
			case ID_FILE_OPEN:
			{
				loadFile(filePathToLoad);
				break;
			}
			default:
			{
				break;
			}
		}
	} else {
		switch( HIWORD(wParam) ) {
			case EN_CHANGE:
			{
				changed = true;
				break;
			}
			// Здесь будет меню, но пока его нет.
			// Здесь будет акселлератор, но пока его нет.
		}
	}
}

void YaPyN_Editor::saveFile(std::string pathToFile)
{
}

void YaPyN_Editor::loadFile(std::string pathToFile)
{
}

void YaPyN_Editor::CreateToolbar()
{
	/*
	TBBUTTON tbb[] = {
		{ STD_FILENEW, ID_FILE_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ STD_FILEOPEN, ID_FILE_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ STD_FILESAVE, ID_FILE_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
	};

	hWndToolBar = CreateToolbarEx(handle, WS_CHILD | WS_VISIBLE | CCS_TOP, 1,
		0, HINST_COMMCTRL, IDB_STD_SMALL_COLOR, tbb, _countof(tbb), 0, 0, 0, 0, sizeof(TBBUTTON));

	//DWORD backgroundColor = GetSysColor(COLOR_BTNFACE);
	//COLORMAP colorMap;
	//colorMap.from = RGB(100, 100, 100);
	//colorMap.to = backgroundColor;
	HBITMAP hbm = CreateMappedBitmap(::GetModuleHandle(0), IDB_BITMAP1, 0, NULL, 1);
	TBADDBITMAP tb;
	tb.hInst = NULL;
	tb.nID = reinterpret_cast<UINT_PTR>(hbm);
	int imageIndex = SendMessage(hWndToolBar, TB_ADDBITMAP, 0, reinterpret_cast<LPARAM>(&tb));
	TBBUTTON tbButtonsAdd[] =
	{
		{ imageIndex, ID_CELL_ADD, TBSTATE_ENABLED, BTNS_BUTTON },
		{ STD_DELETE, ID_CELL_DELETE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
	};
	SendMessage(hWndToolBar, TB_ADDBUTTONS, _countof(tbButtonsAdd), reinterpret_cast<LPARAM>(tbButtonsAdd));
	*/
}

LRESULT YaPyN_Editor::windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( message == WM_NCCREATE ) {
		YaPyN_Editor* window = reinterpret_cast<YaPyN_Editor*> ((reinterpret_cast<CREATESTRUCT*>(lParam))->lpCreateParams);
		SetLastError(0);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG>(window));
		if( GetLastError() ) {
			return GetLastError();
		}
		window->OnNCCreate(hwnd);
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	YaPyN_Editor* window = reinterpret_cast<YaPyN_Editor*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	switch( message ) {
		case WM_CREATE:
		{
			window->OnCreate();
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		case WM_SIZE:
		{
			window->OnSize(wParam);
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		case WM_CLOSE:
		{
			if( window->OnClose() ) {
				::PostQuitMessage(0);
				return DefWindowProc(hwnd, message, wParam, lParam);
			} else {
				return 0;
			}
		}
		case WM_COMMAND:
		{
			window->OnCommand(hwnd, message, wParam, lParam);
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		case WM_DESTROY:
		{
			window->OnDestroy();
			return SuccessDestroyWindowValue;
		}
		default:
		{
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
	}
}
