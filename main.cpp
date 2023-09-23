#include <windows.h>
#include <cmath>
#include <iostream>
#include <chrono>

unsigned int itime = 0;
uint64_t time_since_launch = 0;
const WORD field_width = 512, field_height = 512;

float clamp(float value, float minimum, float maximum) {
	return fmaxf(minimum, fminf(value, maximum));
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
	}

	return DefWindowProcW(hWnd, msg, wParam, lParam);
}

void DrawInWindow(HDC hDC, uint32_t deltaTime) {
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
				blue	= (int)clamp(sinf(((float)time_since_launch / 200.0f)) * 255.0f, 0, 255);
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

	std::chrono::milliseconds old_time = duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
		);

	uint32_t delta_time = 0;
	
	MSG msg;
	while (1) {
		old_time = duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
			);

		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
			if (msg.message == WM_QUIT) break;
		} else {
			DrawInWindow(hDC, delta_time);
		}

		itime++;

		time_since_launch += delta_time;

		std::chrono::milliseconds new_time = duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
			);
		delta_time = new_time.count() - old_time.count();
		//std::cout << delta_time << '\n';
	}

	return 0;
}
