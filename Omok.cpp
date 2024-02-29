#include <iostream>
#include <string> 
#include <cstdlib> 
#include <ctime>
#include <string.h>
#include <stdio.h>
#include <deque>
#include <algorithm>
#include <Windows.h>
#include <vector>

using namespace std;

#define X_LINE		19
#define Y_LINE		19

#define WHITE_SPACE 50

#define INTERVAL	30
#define RADIUS		(INTERVAL / 2)

#define POS(x)		(WHITE_SPACE + (x) * INTERVAL)

unsigned int dol[Y_LINE][X_LINE];
unsigned int order = 1;
unsigned int game_mode = 0;
pair<int, int> computer_dol;

void drawMap(HWND hand);
void checkNear(int y, int x, vector<vector<int>>& visited);
pair<int, int> whiteAI();
pair<int, int> buttonDown(HWND hwnd, int x, int y);
int checkLineValue(pair<int, int> loc);
LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
int WINAPI main(HINSTANCE hIns, HINSTANCE hPrevIns, LPSTR lpCmdLine, int nCmdShow);


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

void checkNear(int y, int x, vector<vector<int>>& visited) {
	int dx[8] = {1, 1, 1, 0, 0, -1, -1, -1};
	int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
	int max_value = 0;
	for (int i = 0; i < 8; i++) {
		int newx = x + dx[i];
		int newy = y + dy[i];
		if (newx < 0 || newx >= X_LINE || newy < 0 || newy >= Y_LINE) {
			continue;
		}
		if (dol[newy][newx] != 0) {
			continue;
		}
		dol[newy][newx] = order;
		int t = checkLineValue(pair<int, int>(newx, newy));
		dol[newy][newx] = 0;
		visited[newy][newx] = max(visited[newy][newx], t);
	}
	

}

//현재 놓여있는 돌들의 점수를 계산해서 위치 파악하기?
pair<int, int> whiteAI() {
	vector<vector<int>> visited(X_LINE, vector<int> (Y_LINE, 0));
	for (int i = 0; i < Y_LINE; i++) {
		for (int j = 0; j < X_LINE; j++) {
			if (dol[i][j] == 0) {
				continue;
			}
			checkNear(i, j, visited);
			if (order == 1) {
				order = 2;
			}
			else {
				order = 1;
			}
			checkNear(i, j, visited);
			if (order == 1) {
				order = 2;
			}
			else {
				order = 1;
			}
		}
	}
	int max_value = 0;
	vector<pair<int, int>> marking;

	for (int i = 0; i < Y_LINE; i++) {
		for (int j = 0; j < X_LINE; j++) {
			cout << visited[i][j] << " ";
			if (visited[i][j] > max_value) {
				marking.clear();
				marking.push_back(pair<int, int>(i, j));
				max_value = visited[i][j];
			}
			else if (visited[i][j] == max_value) {
				marking.push_back(pair<int, int>(i, j));
			}
		}
		cout << endl;
	}

	int markingsize = marking.size();
	int choose = rand() % markingsize;

	return { marking[choose].first, marking[choose].second };
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

				InvalidateRect(hwnd, NULL, TRUE);
				return pair<int, int>(dol_x, dol_y);
			}
		}

	}

	return pair<int, int>(-1, -1);
}

// 게임오버를 검사하는 함수에서, 컴퓨터가 알을 놓기위한 점수 계산방식으로 바꿈
int checkLineValue(pair<int, int> loc) {
	if (loc.first == -1 && loc.second == -1) {
		return 0;
	}
	int x = loc.first;
	int y = loc.second;
	int dol_color = dol[y][x];
	int garo = 1;
	int sero = 1;
	int tentofive = 1;
	int seventotwo = 1;
	int count = 0;
	int max_value = 0;

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

	max_value = max(max_value, garo);

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

	max_value = max(max_value, sero);

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

	max_value = max(max_value, tentofive);

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

	max_value = max(max_value, seventotwo);
	if (max_value == 4) {
		max_value = 7;
	}
	else if (max_value == 5) {
		max_value = 10;
	}
	count = 0;

	return max_value;
}

/*
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
*/

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	
	if (umsg == WM_PAINT) {
		drawMap(hwnd);
	}
	else if (umsg == WM_LBUTTONDOWN) {
		
		auto loc = buttonDown(hwnd, (int)LOWORD(lparam), (int)HIWORD(lparam));
		if (game_mode == 1) {
			unsigned int value = checkLineValue(loc);
			if (order == 2 && value == 10) {
				MessageBox(hwnd, L"흑돌승리", L"Black Win!", MB_OK);
				memset(dol, 0, sizeof(dol));
				order = 1;
				InvalidateRect(hwnd, NULL, TRUE);
			}
			else if (order == 1 && value == 10) {
				MessageBox(hwnd, L"백돌승리", L"White Win!", MB_OK);
				memset(dol, 0, sizeof(dol));
				order = 1;
				InvalidateRect(hwnd, NULL, TRUE);
			}
		}
		else {
			unsigned int value = checkLineValue(loc);
			if (value == 10) {
				MessageBox(hwnd, L"흑돌승리", L"Black Win!", MB_OK);
				memset(dol, 0, sizeof(dol));
				order = 1;
				InvalidateRect(hwnd, NULL, TRUE);
			}
			else {
				if (order == 1) {
					order++;
				}
				else {
					order--;
				}
				pair<int, int> com = whiteAI();
				if (order == 1) {
					order++;
				}
				else {
					order--;
				}

				computer_dol.first = com.first;
				computer_dol.second = com.second;

				dol[computer_dol.first][computer_dol.second] = 2;
				cout << computer_dol.first << computer_dol.second << endl;
				value = checkLineValue(computer_dol);
				cout << value << endl;
				if (value == 10) {
					MessageBox(hwnd, L"백돌승리", L"Black Win!", MB_OK);
					memset(dol, 0, sizeof(dol));
					order = 1;
					InvalidateRect(hwnd, NULL, TRUE);
				}
			}


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