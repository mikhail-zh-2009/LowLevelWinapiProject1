#include <windows.h>

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return DefWindowProcW(hWnd, msg, wParam, lParam);
}

int main() {
	WNDCLASSW wndClass;
	memset(&wndClass, 0, sizeof(WNDCLASSW));
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = L"window_class";
	RegisterClassW(&wndClass);

	HWND wnd;
	wnd = CreateWindow(L"window_class", L"Hello World!", WS_OVERLAPPEDWINDOW, 0, 0, 400, 400, NULL, NULL, NULL, NULL);
	ShowWindow(wnd, SW_SHOWNORMAL);

	MSG msg;
	while (1) {
		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	return 0;
}
