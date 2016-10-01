#pragma comment(lib, "comctl32.lib")

#include "YaPyN_Editor.h"

// Временные путь для сохранения/загрузки.
const std::string filePathToLoad = "file.txt";
const std::string filePathToSave = "file.txt";

const int YaPyN_Editor::sizeBetweenCells = 10;
const int YaPyN_Editor::marginLeftRightCells = 10;

YaPyN_Editor::YaPyN_Editor()
{
	handleMainWindow = 0;
	handleToolbar = 0;
	changed = false;
	childrensWindow.resize(0);
	activeCell = childrensWindow.end();
	buttonsBitmaps.clear();
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
		WS_EX_OVERLAPPEDWINDOW | WS_SIZEBOX | WS_SYSMENU | WS_VSCROLL,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		GetModuleHandle(0),
		this);

	createToolbar();
	checkHandle(handleMainWindow);
	return (handleMainWindow != 0);
}

void YaPyN_Editor::Show(int cmdShow)
{
	ShowWindow(handleMainWindow, cmdShow);
	for( auto window = childrensWindow.begin(); window != childrensWindow.end(); ++window ) {
		window->Show(cmdShow);
	}
}

void YaPyN_Editor::OnNCCreate(HWND hwnd)
{
	handleMainWindow = hwnd;
}

void YaPyN_Editor::OnCreate()
{
	createCell();
	createCell();
	createCell();
	createCell();
	createCell();
}

void YaPyN_Editor::OnSize()
{
	InvalidateRect(handleMainWindow, NULL, FALSE);
	PAINTSTRUCT paintStruct;
	BeginPaint(handleMainWindow, &paintStruct);

	HBRUSH brush;
	brush = CreateSolidBrush(RGB(100, 150, 200));
	FillRect(paintStruct.hdc, &paintStruct.rcPaint, brush);
	EndPaint(handleMainWindow, &paintStruct);

	RECT rect;
	::GetClientRect(handleMainWindow, &rect);

	RECT toolbarRect;
	::GetClientRect(handleToolbar, &toolbarRect);

	int currentTop = rect.top + (toolbarRect.bottom - toolbarRect.top) + sizeBetweenCells;
	for( auto window = childrensWindow.begin(); window != childrensWindow.end(); ++window ) {
		LONG leftBorder = rect.left + marginLeftRightCells;
		LONG width = rect.right - rect.left - 2 * marginLeftRightCells;
		
		SetWindowPos(window->getHandle(), HWND_TOP, leftBorder, currentTop, width, window->getHeight(), 0);
		currentTop += sizeBetweenCells + window->getHeight();
	}

	SendMessage(handleToolbar, TB_AUTOSIZE, 0, 0);
}

void YaPyN_Editor::OnDestroy()
{
	PostQuitMessage(SuccessDestroyWindowValue);
}

bool YaPyN_Editor::OnClose()
{
	if( changed ) {
		switch( MessageBox(handleMainWindow, L"Вы ввели текст. Сохранить?", L"Завершение работы", MB_YESNOCANCEL | MB_ICONWARNING ))
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
		//TODO: Вернуть позже. Мешается..
		//switch( MessageBox(handleMainWindow, L"Вы действительно хотите выйти?", L"Завершение работы", MB_YESNO | MB_ICONWARNING )) {
		//	case IDNO:
		//	{
		//		return false;
		//	}
		//	case IDYES:
		//	{
		//		return true;
		//	}
		//}
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
			case ID_CELL_ADD:
			{
				createCell();
				break;
			}
			case ID_CELL_DELETE:
			{
				if( MessageBox(handleMainWindow, L"Вы действительно хотите удалить ячейку?",
					L"Удаление ячейки", MB_YESNO | MB_ICONWARNING) == IDYES ) {
					deleteCell();
				}
				break;
			}
			case ID_CELL_UP:
			{
				moveUpCell();
				break;
			}
			case ID_CELL_DOWN:
			{
				moveDownCell();
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
			case EN_UPDATE:
			{
				resizeCell(reinterpret_cast<HWND>(lParam));
			}
			// Здесь будет меню, но пока его нет.
			// Здесь будет акселлератор, но пока его нет.
		}
	}
}

void YaPyN_Editor::createToolbar() {

	const int bitmapSize = 16;

	HINSTANCE hInstance = ::GetModuleHandle(0);

	handleToolbar = CreateToolbarEx(handleMainWindow, WS_CHILD | WS_VISIBLE | CCS_TOP, 1,
				0, HINST_COMMCTRL, IDB_STD_SMALL_COLOR, NULL, 0, 0, 0, 0, 0, sizeof(TBBUTTON));

	//Enable multiple image lists
	SendMessage(handleToolbar, CCM_SETVERSION, (WPARAM)5, 0);

	//Add icons from default imagelist
	TBBUTTON tbb_buildin[] = {
		{ STD_FILENEW, ID_FILE_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ STD_FILEOPEN, ID_FILE_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ STD_FILESAVE, ID_FILE_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
	};
	SendMessage(handleToolbar, (UINT)TB_ADDBUTTONS, _countof(tbb_buildin), (LPARAM)&tbb_buildin);

	//Create custom imagelist
	HIMAGELIST hImageList = ImageList_Create(bitmapSize, bitmapSize, ILC_COLOR16 | ILC_MASK, 0, 0);
	ImageList_Add(hImageList, LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_PLUS)), NULL);
	ImageList_Add(hImageList, LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_DELETE)), NULL);
	ImageList_Add(hImageList, LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_UP)), NULL);
	ImageList_Add(hImageList, LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_DOWN)), NULL);
	SendMessage(handleToolbar, TB_SETIMAGELIST, (WPARAM)1, (LPARAM)hImageList);

	TBBUTTON tbb[] =
	{
		{ MAKELONG(0, 1), ID_CELL_ADD, TBSTATE_ENABLED, TBSTYLE_BUTTON },
		{ MAKELONG(1, 1), ID_CELL_DELETE, TBSTATE_ENABLED, TBSTYLE_BUTTON },
		{ MAKELONG(2, 1), ID_CELL_UP, TBSTATE_ENABLED, TBSTYLE_BUTTON },
		{ MAKELONG(3, 1), ID_CELL_DOWN, TBSTATE_ENABLED, TBSTYLE_BUTTON },
	};

	SendMessage(handleToolbar, (UINT)TB_ADDBUTTONS, _countof(tbb), (LPARAM)&tbb);
}

void YaPyN_Editor::saveFile(std::string pathToFile)
{
}

void YaPyN_Editor::loadFile(std::string pathToFile)
{
}

void YaPyN_Editor::createCell()
{
	activeCell = childrensWindow.emplace(activeCell == childrensWindow.end() ? childrensWindow.end() : ++activeCell, CellWindow());
	activeCell->Create(handleMainWindow);
	handlesAndCells.insert(std::make_pair(activeCell->getHandle(), activeCell));
	SendMessage(handleMainWindow, WM_SIZE, 0, 0);
}

void YaPyN_Editor::deleteCell()
{
	if( activeCell != childrensWindow.end() ) {
		HWND hwnd = activeCell->getHandle();
		checkHandle(hwnd);
		DestroyWindow(hwnd);
		auto oldCell = activeCell;
		auto nextCell = activeCell;
		++nextCell;
		activeCell = (nextCell != childrensWindow.end()
					? nextCell
					: (activeCell != childrensWindow.begin()
						? --activeCell
						: childrensWindow.end()));
		childrensWindow.erase(oldCell);
		handlesAndCells.erase(handlesAndCells.find(hwnd));
		SendMessage(handleMainWindow, WM_SIZE, 0, 0);
	} else {
		MessageBox(handleMainWindow, L"Выберите ячейку!", L"Не выбрана ячейка", MB_OK | MB_ICONWARNING);
	}
}

void YaPyN_Editor::moveUpCell() {
}

void YaPyN_Editor::moveDownCell() {
}

void YaPyN_Editor::resizeCell(HWND handleCell)
{
	activeCell = handlesAndCells.find(handleCell)->second;

	if( activeCell->changeHeight(getCountsOfStrings(handleCell)) ) {
		SendMessage(handleMainWindow, WM_SIZE, 0, 0);
	}
}

unsigned int YaPyN_Editor::getCountsOfStrings(HWND handleCell)
{
	CellWindow* cell = &*handlesAndCells.find(handleCell)->second;

	unsigned int countOfN = 0;
	unsigned int indexOfN = 0;
	unsigned int countOfLongStrings = 0;

	int length = SendMessage(handleCell, WM_GETTEXTLENGTH, 0, 0);
	std::shared_ptr<wchar_t> text_ptr(new wchar_t[length + 1]);
	wchar_t* text = text_ptr.get();
	SendMessage(handleCell, WM_GETTEXT, length + 1, reinterpret_cast<LPARAM>(text));

	if (text[0] = '\n') {
		++countOfN;
	}
	
	for( int i = 1; i < length; ++i ) {
		if( text[i] == '\n' ) {
			++countOfN;
			indexOfN = i;
		}
	}

	return countOfN;
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
