#include "commons.h"
#include "Table.h"

#include<fstream>
#include<cwchar>
#include<cmath>


constexpr int ROWS = 3;
constexpr int COLUMNS = 6;
constexpr int MIN_WINDOW_WIDTH = COLUMNS * 30;
constexpr int MIN_WINDOW_HEIGHT = 100;
constexpr int PEN_WIDTH = 1;
constexpr int MARGIN = 1;
const std::wstring TEXT = L"Windows — семейство проприетарных операционных систем корпорации Microsoft, ориентированных на применение графического интерфейса при управлении. Изначально Windows была всего лишь графической надстройкой для MS-DOS. По состоянию на август 2014 года под управлением операционных систем семейства Windows по данным ресурса Net Applications работает около 89% персональных компьютеров. Windows работает на платформах x86, x86-64, IA-64 и ARM. Существовали также версии для DEC Alpha, MIPS, PowerPC и SPARC.";

HINSTANCE _applicationHandle;
HWND _mainWindow;
Table* _table;
std::wofstream _log("log.txt");


void fillWindowClass(WNDCLASSEX &windowClass, const std::wstring &className);
HWND createWindow(const std::wstring &className);
LRESULT CALLBACK handleEvent(HWND window, UINT messageType, WPARAM wparam, LPARAM lparam);

LRESULT onClose(HWND window, WPARAM wparam, LPARAM lparam);
LRESULT onDestroy(HWND window, WPARAM wparam, LPARAM lparam);
LRESULT onCreate(HWND window, WPARAM wparam, LPARAM lparam);
LRESULT onPaint(HWND window, WPARAM wparam, LPARAM lparam);
LRESULT onSizing(HWND window, WPARAM wparam, LPARAM lparam);
LRESULT onGetMinMaxInfo(HWND window, WPARAM wparam, LPARAM lparam);

void drawTable(HDC screenDC);
void drawTableText(HDC screenDC);
int getCellWidth();
int getCellHeight();
RECT getCellRect(int row, int column);

int WINAPI WinMain(HINSTANCE applicationHandle,
				   HINSTANCE prevApplicationHandle,
				   LPSTR cmdParams,
				   int startupSizeFlags) {
	_applicationHandle = applicationHandle;
	WNDCLASSEX windowClass = {};
	std::wstring className = L"MainWindowClass";
	fillWindowClass(windowClass, className);
	if (!RegisterClassEx(&windowClass))
		kvp::exitWithErrorMessage(L"RegisterClassEx");

	_mainWindow = createWindow(className);
	if (!_mainWindow)
		kvp::exitWithErrorMessage(L"CreateWindow");

	ShowWindow(_mainWindow, startupSizeFlags);

	MSG message = {};
	while (GetMessage(&message, nullptr, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return static_cast<int>(message.wParam);
}

LRESULT CALLBACK handleEvent(HWND window, UINT messageType, WPARAM wparam, LPARAM lparam) {
	switch (messageType) {
		case WM_CREATE: 
			return onCreate(window, wparam, lparam);
		case WM_PAINT:
			return onPaint(window, wparam, lparam);
		case WM_CLOSE: 
			return onClose(window, wparam, lparam);
		case WM_DESTROY: 
			return onDestroy(window, wparam, lparam);
		case WM_SIZING:
			return onSizing(window, wparam, lparam);
		case WM_SIZE:
			return onSizing(window, wparam, lparam);
		case WM_GETMINMAXINFO:
			return onGetMinMaxInfo(window, wparam, lparam);
		default: 
			return DefWindowProc(window, messageType, wparam, lparam);
	}
}

LRESULT onGetMinMaxInfo(HWND window, WPARAM wparam, LPARAM lparam) {
	MINMAXINFO *minMaxWindowSizeInfo = reinterpret_cast<MINMAXINFO *>(lparam);
	minMaxWindowSizeInfo->ptMinTrackSize.x = MIN_WINDOW_WIDTH;
	minMaxWindowSizeInfo->ptMinTrackSize.y = MIN_WINDOW_HEIGHT;
	return 0;
}

LRESULT onClose(HWND window, WPARAM wparam, LPARAM lparam) {
	DestroyWindow(window);
	return 0;
}

LRESULT onDestroy(HWND window, WPARAM wparam, LPARAM lparam) {
	delete _table;
	_log.close();
	PostQuitMessage(0);
	return 0;
}

LRESULT onCreate(HWND window, WPARAM wparam, LPARAM lparam) {
	RECT clientRect;
	GetClientRect(window, &clientRect);
	_table = new Table(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, ROWS, COLUMNS);
	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLUMNS; j++) {
			int len = (i * COLUMNS + j) + 1;
			_table->getCell(i, j).appendText(TEXT.substr(0, len));
		}
	return 0;
}

LRESULT onPaint(HWND window, WPARAM wparam, LPARAM lparam) {
	HDC screenDC;
	PAINTSTRUCT paintParams;

	screenDC = BeginPaint(window, &paintParams);
	RECT clientRect;
	GetClientRect(window, &clientRect);

	drawTable(screenDC);
	drawTableText(screenDC);

	EndPaint(window, &paintParams);
	return 0;
}

LRESULT onSizing(HWND window, WPARAM wparam, LPARAM lparam) {
	RECT clientRect;
	GetClientRect(window, &clientRect);
	_table->setHeight(clientRect.bottom - clientRect.top);
	_table->setWidth(clientRect.right - clientRect.left);
	return 0;
}

void drawTable(HDC screenDC)
{
	LOGBRUSH brush;
	brush.lbColor = RGB(0, 0, 0);
	brush.lbHatch = 0;
	brush.lbStyle = BS_SOLID;

	auto penHandle = ExtCreatePen(PS_SOLID, PEN_WIDTH, &brush, 0, nullptr);
	auto oldPenHandle = SelectObject(screenDC, penHandle);


	int deltaX = PEN_WIDTH + getCellWidth();
	int deltaY = PEN_WIDTH + getCellHeight();

	for (int i = 0; i < _table->getRows(); i++) {
		int y = i*deltaY;

		MoveToEx(screenDC, 0, y, nullptr);
		LineTo(screenDC, _table->getWidth(), y);
	}

	for (int i = 0; i < _table->getColumns(); i++) {
		int x = i*deltaX;

		MoveToEx(screenDC, x, 0, nullptr);
		LineTo(screenDC, x, _table->getHeight());
	}

	SelectObject(screenDC, oldPenHandle);
	DeleteObject(penHandle);
	
}

void drawTableText(HDC screenDC)
{
	_log << _table->getWidth() << L' ' << _table->getHeight() << L'\n';
	for (int i = 0; i < _table->getRows(); i++)
		for (int j = 0; j < _table->getColumns(); j++) {
			RECT cellRect = getCellRect(i, j);
			wchar_t* text = const_cast<wchar_t *>(_table->getCell(i, j).getText().c_str());
			
			int charWidth = (cellRect.right - cellRect.left) / (_table->getCell(i, j).getText().length());
			int charHeight = (cellRect.bottom - cellRect.top);

//			_log << L"left=" << cellRect;

			auto fontHandle = CreateFont(charHeight, charWidth, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH, L"Arial Bold");
			auto oldFontHandle = SelectObject(screenDC, fontHandle);

			DrawTextExW(screenDC, 
						text, 
						_table->getCell(i, j).getText().length(), 
						&cellRect, 
						DT_SINGLELINE|DT_CENTER|DT_VCENTER, 
						nullptr);

			SelectObject(screenDC, oldFontHandle);
			DeleteObject(fontHandle);
		}
}

int getCellWidth() {
	return (_table->getWidth() - PEN_WIDTH * (_table->getColumns())) / _table->getColumns();
}
int getCellHeight() {
	return (_table->getHeight() - PEN_WIDTH * (_table->getRows())) / _table->getRows();
}

RECT getCellRect(int row, int column) {
	RECT cellRect;
	cellRect.left = column * (getCellWidth() + PEN_WIDTH) + PEN_WIDTH;
	cellRect.right = cellRect.left + getCellWidth() - 1;
	cellRect.top = row * (getCellHeight() + PEN_WIDTH) + PEN_WIDTH;
	cellRect.bottom = cellRect.top + getCellHeight() - 1;
	return cellRect;
}

void fillWindowClass(WNDCLASSEX &windowClass, const std::wstring &className) {
	windowClass.cbSize = sizeof(windowClass);
	windowClass.style = CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;
	windowClass.lpfnWndProc = handleEvent;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = _applicationHandle;
	windowClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	windowClass.lpszMenuName = nullptr;
	windowClass.lpszClassName = className.c_str();
	windowClass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
}

HWND createWindow(const std::wstring &className) {
	return CreateWindowEx(WS_EX_ACCEPTFILES,
						  className.c_str(),
  						  className.c_str(),
						  WS_OVERLAPPEDWINDOW,
						  CW_USEDEFAULT,
						  CW_USEDEFAULT,
						  CW_USEDEFAULT,
						  CW_USEDEFAULT,
						  nullptr,
						  nullptr,
						  _applicationHandle,
						  nullptr);
}
