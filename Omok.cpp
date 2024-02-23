#include <iostream>
#include <string> 
#include <cstdlib> 
#include <ctime>
#include <string.h>
#include <stdio.h>
#include <deque>
#include <algorithm>
#include <Windows.h>

using namespace std;

#define X_LINE		19
#define Y_LINE		19

#define WHITE_SPACE 50

#define INTERVAL	30
#define RADIUS		(INTERVAL / 2)

#define POS(x)		(WHITE_SPACE + (x) * INTERVAL)

unsigned int dol[Y_LINE][X_LINE];
unsigned int order = 1;
unsigned int ending = 0;
unsigned int game_mode = 0;

void drawMap(HWND hwnd) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	for (int i = 0; i < X_LINE; i++) {
		MoveToEx(hdc, POS(i), POS(0), NULL);
		LineTo(hdc, POS(i), POS(Y_LINE - 1));
	}

	for (int i = 0; i < Y_LINE; i++) {
		MoveToEx(hdc, POS(0), POS(i), NULL);
		LineTo(hdc, POS(X_LINE - 1), POS(i));
	}

	for (int i = 0; i < Y_LINE; i++) {
		for (int j = 0; j < X_LINE; j++) {
			if (dol[i][j] > 0) {
				if (dol[i][j] == 1) {
					SelectObject(hdc, GetStockObject(BLACK_BRUSH));
				}
				else {
					SelectObject(hdc, GetStockObject(WHITE_BRUSH));
				}
				Ellipse(hdc, POS(j) - RADIUS, POS(i) - RADIUS, POS(j) + RADIUS, POS(i) + RADIUS);
			}
		}
	}

	EndPaint(hwnd, &ps);
}

//minimax 알고리즘 적용하여 ai의 돌 놓기
//ai가 이전에 놓았던 자리와, 내가 놓았던 자리에서 비교하기 되나?
void whiteAI(int x, int y) {
	
}

pair<int, int> buttonDown(HWND hwnd, int x, int y) {
	if (x > POS(0) - RADIUS && y > POS(0) - RADIUS &&
		x < POS(X_LINE - 1) + RADIUS && y < POS(Y_LINE - 1) + RADIUS) {
		int dol_x = (x - WHITE_SPACE + RADIUS) / INTERVAL;
		int dol_y = (y - WHITE_SPACE + RADIUS) / INTERVAL;
		if (game_mode == 1) {
			if (dol[dol_y][dol_x] == 0) {
				dol[dol_y][dol_x] = order;
				if (order == 1) {
					order++;
				}
				else {
					order--;
				}
				InvalidateRect(hwnd, NULL, TRUE);

				return pair<int, int>(dol_x, dol_y);
			}
		}
		else {
			if (dol[dol_y][dol_x] == 0) {
				dol[dol_y][dol_x] = order;
				whiteAI(dol_x, dol_y);
				InvalidateRect(hwnd, NULL, TRUE);
				return pair<int, int>(dol_x, dol_y);
			}
		}

	}

	return pair<int, int>(-1, -1);
}

int checkGameOver(pair<int, int> loc) {
	if (loc.first == -1 && loc.second == -1) {
		return 0 ;
	}
	int x = loc.first;
	int y = loc.second;
	int dol_color = dol[y][x];
	int garo = 1;
	int sero = 1;
	int tentofive = 1;
	int seventotwo = 1;
	int count = 0;

	for (int i = x - 1; i >= 0; i--) {
		if (count++ > 5) {
			break;
		}
		if (dol[y][i] == dol_color) {
			garo++;
		}
		else {
			break;
		}
	}

	for (int i = x + 1; i <= X_LINE; i++) {
		if (count++ > 5) {
			break;
		}
		if (dol[y][i] == dol_color) {
			garo++;
		}
		else {
			break;
		}
	}

	if (garo >= 5) {
		return dol_color;
	}

	count = 0;

	for (int i = y - 1; i >= 0; i--) {
		if (count++ > 5) {
			break;
		}
		if (dol[i][x] == dol_color) {
			sero++;
		}
		else {
			break;
		}
	}

	for (int i = y + 1; i <= Y_LINE; i++) {
		if (count++ > 5) {
			break;
		}
		if (dol[i][x] == dol_color) {
			sero++;
		}
		else {
			break;
		}
	}

	if (sero >= 5) {
		return dol_color;
	}

	count = 0;

	int temp_x = x - 1;

	for (int i = y - 1; i >= 0; i--) {
		if (temp_x < 0 || count++ > 5) {
			break;
		}
		if (dol[i][temp_x--] == dol_color) {
			tentofive++;
		}
		else {
			break;
		}
	}

	temp_x = x + 1;

	for (int i = y + 1; i <= Y_LINE; i++) {
		if (temp_x > X_LINE || count++ > 5) {
			break;
		}
		if (dol[i][temp_x++] == dol_color) {
			tentofive++;
		}
		else {
			break;
		}
	}

	if (tentofive >= 5) {
		return dol_color;
	}

	count = 0;

	temp_x = x + 1;
	for (int i = y - 1; i >= 0; i--) {
		if (temp_x > X_LINE || count++ > 5) {
			break;
		}
		if (dol[i][temp_x++] == dol_color) {
			seventotwo++;
		}
		else {
			break;
		}
	}
	temp_x = x - 1;
	for (int i = y + 1; i <= Y_LINE; i++) {
		if (temp_x < 0 || count++ > 5) {
			break;
		}
		if (dol[i][temp_x--] == dol_color) {
			seventotwo++;
		}
		else {
			break;
		}
	}

	if (seventotwo >= 5) {
		return dol_color;
	}

	count = 0;

	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	
	if (umsg == WM_PAINT) {
		drawMap(hwnd);
	}
	else if (umsg == WM_LBUTTONDOWN) {
		
		auto loc = buttonDown(hwnd, (int)LOWORD(lparam), (int)HIWORD(lparam));
		ending = checkGameOver(loc);
		if (ending == 1) {
			MessageBox(hwnd, L"흑돌승리", L"Black Win!", MB_OK);
			memset(dol, 0, sizeof(dol));
			ending = 0;
			order = 1;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else if (ending == 2) {
			MessageBox(hwnd, L"백돌승리", L"White Win!", MB_OK);
			memset(dol, 0, sizeof(dol));
			ending = 0;
			order = 1;
			InvalidateRect(hwnd, NULL, TRUE);
		}

	}
	else if (umsg == WM_DESTROY) {
		PostQuitMessage(0);
	}

	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

int WINAPI main(HINSTANCE hIns, HINSTANCE hPrevIns, LPSTR lpCmdLine, int nCmdShow) {
	
	WNDCLASS wc;
	wchar_t class_name[] = L"omok";
	
	HBRUSH background = CreateSolidBrush(RGB(250, 177, 45));

	wc.cbClsExtra = NULL;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)background;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hIns;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = class_name;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);

	HWND hwnd = CreateWindow(class_name, class_name, WS_OVERLAPPEDWINDOW, 0, 0, 
		2 * WHITE_SPACE + Y_LINE * INTERVAL, 2 * WHITE_SPACE + X_LINE * INTERVAL, 
		NULL, NULL, hIns, NULL);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	
	if (MessageBox(hwnd, L"게임 모드를 선택하세요 \n YES : 1vs1 대전 / NO : vs AI", L"게임모드 선택", MB_YESNO) == IDYES) {
		game_mode = 1;
	}
	else {
		game_mode = 2;
	}
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DeleteObject(background);

	return (int)msg.wParam;

}