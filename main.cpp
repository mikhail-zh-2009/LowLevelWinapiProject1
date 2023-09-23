#include <windows.h>
#include <cmath>
#include <iostream>

unsigned int itime = 0;
const WORD field_width = 192, field_height = 192;

float clamp(float value, float minimum, float maximum) {
	return fmaxf(minimum, fminf(value, maximum));
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
	}

	return DefWindowProcW(hWnd, msg, wParam, lParam);
}

void DrawInWindow(HDC hDC) {
	RECT r = { 0, 0, field_width, field_height };

	BITMAPINFO bif;
	ZeroMemory(&bif, sizeof(BITMAPINFO));

	bif.bmiHeader.biSize = sizeof(bif);

	RGBQUAD* im = new RGBQUAD[r.right * r.bottom];

	for (int i(0); i < r.right; i++) {
		for (int ii(0); ii < r.bottom; ii++) {
			float x = (float)i;
			float y = (float)ii;
			int
				red		= (int)clamp(x, 0, 255),
				green	= (int)clamp(y, 0, 255),
				blue	= (int)clamp(sinf(((float)itime / 100.0f) * 0.5f + 0.5f) * 255.0f, 0, 255);
			im[r.right * i + ii].rgbBlue = blue;
			im[r.right * i + ii].rgbRed = red;
			im[r.right * i + ii].rgbGreen = green;
		}
	}

	bif.bmiHeader.biHeight = -r.bottom;
	bif.bmiHeader.biWidth = r.right;
	bif.bmiHeader.biSizeImage = ((bif.bmiHeader.biWidth * 24 + 31) & ~31) / 8 * bif.bmiHeader.biHeight;
	bif.bmiHeader.biPlanes = 1;
	bif.bmiHeader.biBitCount = sizeof(RGBQUAD) * 8;

	SetDIBitsToDevice(hDC, 0, 0, r.right, r.bottom, 0, 0, 0, r.bottom, im, &bif, DIB_PAL_COLORS);

	delete[] im;
}

int main() {
	WNDCLASSW wndClass;
	memset(&wndClass, 0, sizeof(WNDCLASSW));
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = L"window_class";
	RegisterClassW(&wndClass);

	HWND wnd;
	wnd = CreateWindow(L"window_class", L"Hello World!", WS_OVERLAPPEDWINDOW, 0, 0, field_width + 16, field_height + 39, NULL, NULL, NULL, NULL);
	ShowWindow(wnd, SW_SHOWNORMAL);
	HDC hDC = GetDC(wnd);

	MSG msg;
	while (1) {
		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
			if (msg.message == WM_QUIT) break;
		} else {
			DrawInWindow(hDC);
		}
		itime++;
	}

	return 0;
}
