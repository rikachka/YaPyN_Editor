#include "YaPyN_Editor.h"

// Временные путь для сохранения/загрузки.
std::string filePathToLoad = "file.txt";
std::string filePathToSave = "file.txt";

YaPyN_Editor::YaPyN_Editor()
{
	handle = 0;
	changed = false;
}

bool YaPyN_Editor::RegisterClass()
{
	WNDCLASSEX windowClass;
	::ZeroMemory(&windowClass, sizeof(windowClass));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = YaPyN_Editor::windowProc;
	windowClass.hInstance = GetModuleHandle(0);
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

	/*
	CreateToolbar();
	*/
	if( handle == 0 ) {
		MessageBox(handle, L"Error: CreateWindowEx return 0", NULL, MB_OK);
	}
	return (handle != 0);
}

void YaPyN_Editor::Show(int cmdShow)
{
	ShowWindow(handle, cmdShow);
}

void YaPyN_Editor::OnNCCreate(HWND hwnd)
{
	handle = hwnd;
}

void YaPyN_Editor::OnCreate()
{
}

//Действия, которые будут происходить с дочерними окнами при изменении размера окна.
void YaPyN_Editor::OnSize()
{
}

void YaPyN_Editor::OnDestroy()
{
	PostQuitMessage(SuccessDestroyWindowValue);
}

bool YaPyN_Editor::OnClose()
{
	if( changed ) {
		switch (MessageBox(handle, L"Вы ввели текст. Сохранить?", L"Завершение работы", MB_YESNOCANCEL))
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
		switch (MessageBox(handle, L"Do you really want to leave?", L"Exit", MB_YESNO | MB_ICONWARNING)) {
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
		/*
		switch( LOWORD(wParam) ) {
			case ID_FILE_EXIT:
			{
				OnClose();
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
		*/
	}
}

void YaPyN_Editor::saveFile(std::string pathToFile)
{
}

void YaPyN_Editor::loadFile(std::string pathToFile)
{
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
			window->OnSize();
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
