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
#include <deque>

using namespace std;
// 82 , 45
#define WIDTH_LINE		41
#define HEIGHT_LINE		22

#define WHITE_SPACE 50

#define INTERVAL	40
#define RADIUS		(INTERVAL / 2)

#define POS(x)		(WHITE_SPACE + (x) * INTERVAL)



#define PII			pair<int, int>

// 줄일수록 최대 속도가 빨라짐
#define MAX_SPEED	70
#define START_SPEED	300

#define RIGHT		1
#define LEFT		2
#define UP			3
#define DOWN		4

#define HEAD		10
#define BODY		20
#define APPLE		30

unsigned int screen[HEIGHT_LINE][WIDTH_LINE];
unsigned int direction = RIGHT;
unsigned int start = 0;
unsigned int speed = START_SPEED;
PII snake_head = { HEIGHT_LINE / 2 - 1, WIDTH_LINE / 2 - 1 };
deque<PII> snake_body;
PII apple = {HEIGHT_LINE / 2 - 1, WIDTH_LINE / 2 + 2};
unsigned int score = 0;
WCHAR word[1024];

void drawMap(HWND hand);
void moveSnake(HWND hwnd);
// 사과와 충돌, 몸통과 충돌, 벽과 충돌 감지
void checkApple(HWND hwnd);
void speedUp(HWND hwnd);
int checkBody(HWND hwnd);
int checkWall(HWND hwnd);
void gameOver(HWND hwnd);


VOID CALLBACK TimerProc(HWND hwnd, UINT, UINT_PTR idEvent, DWORD);
LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
int WINAPI main(HINSTANCE hIns, HINSTANCE hPrevIns, LPSTR lpCmdLine, int nCmdShow);




void drawMap(HWND hwnd) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	int check_line = 0;
	// 점수
	wsprintfW(word, L"score : %d", score * 10);
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, POS(WIDTH_LINE) - WHITE_SPACE - INTERVAL * 2, WHITE_SPACE / 2, word, lstrlen(word));


	//외각선
	HPEN hPen = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	MoveToEx(hdc, POS(0), POS(0), NULL);
	LineTo(hdc, POS(0), POS(HEIGHT_LINE - 1));
	MoveToEx(hdc, POS(WIDTH_LINE - 1), POS(0), NULL);
	LineTo(hdc, POS(WIDTH_LINE - 1), POS(HEIGHT_LINE - 1));

	MoveToEx(hdc, POS(0), POS(0), NULL);
	LineTo(hdc, POS(WIDTH_LINE - 1), POS(0));
	MoveToEx(hdc, POS(0), POS(HEIGHT_LINE - 1), NULL);
	LineTo(hdc, POS(WIDTH_LINE - 1), POS(HEIGHT_LINE - 1));
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
	/*
	hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	hOldPen = (HPEN)SelectObject(hdc, hPen);
	//내각선
	for (int i = 1; i < WIDTH_LINE - 1; i++) {
		MoveToEx(hdc, POS(i), POS(0), NULL);
		LineTo(hdc, POS(i), POS(HEIGHT_LINE - 1));
	}

	for (int i = 1; i < HEIGHT_LINE - 1; i++) {
		MoveToEx(hdc, POS(0), POS(i), NULL);
		LineTo(hdc, POS(WIDTH_LINE - 1), POS(i));
	}
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
	*/
	for (int i = 0; i < HEIGHT_LINE; i++) {
		for (int j = 0; j < WIDTH_LINE; j++) {
			if (screen[i][j] > 0) {
				HDC h_dc = GetDC(hwnd);
				SelectObject(h_dc, GetStockObject(DC_BRUSH));
				if (screen[i][j] == HEAD) {
					SetDCBrushColor(h_dc, RGB(0, 128, 0));
					Ellipse(h_dc, POS(j) - RADIUS, POS(i) - RADIUS, POS(j) + RADIUS, POS(i) + RADIUS);
					
				}
				else if (screen[i][j] == BODY) {
					SetDCBrushColor(h_dc, RGB(0, 255, 0));
					Ellipse(h_dc, POS(j) - RADIUS, POS(i) - RADIUS, POS(j) + RADIUS, POS(i) + RADIUS );
					
				}
				else if (screen[i][j] == APPLE) {
					SetDCBrushColor(h_dc, RGB(255, 0, 0));
					Ellipse(h_dc, POS(j) - RADIUS, POS(i) - RADIUS, POS(j) + RADIUS, POS(i) + RADIUS);
					
				}
				ReleaseDC(hwnd, h_dc);
			}
		}
	}

	TextOut(hdc, POS(0), POS(HEIGHT_LINE), L"w : up / s : down / a : left / d : right", lstrlen(L"w : up / s : down / a : left / d : right"));

	EndPaint(hwnd, &ps);
}

void CALLBACK moveSnake(HWND hwnd) {
	snake_body.push_back(snake_head);
	screen[snake_head.first][snake_head.second] = BODY;
	switch (direction) {
	case LEFT:
		snake_head.second -= 1;
		break;
	case RIGHT:
		snake_head.second += 1;
		break;
	case UP:
		snake_head.first -= 1;
		break;
	case DOWN:
		snake_head.first += 1;
		break;
	}

	if (snake_head != apple && !snake_body.empty() && snake_body.size() > score) {
		PII fade = snake_body.front();
		snake_body.pop_front();
		screen[fade.first][fade.second] = 0;
	}
	screen[snake_head.first][snake_head.second] = HEAD;



}

void checkApple(HWND hwnd) {
	if (snake_head == apple) {
		while (snake_head == apple || 
			find(snake_body.begin(), snake_body.end(), apple) != snake_body.end()) {
			apple.first = rand() % (HEIGHT_LINE - 2) + 1;
			apple.second = rand() % (WIDTH_LINE - 2) + 1;
		}
		screen[apple.first][apple.second] = APPLE;
		score++;
		speedUp(hwnd);

	}

	return;
}

void speedUp(HWND hwnd) {
	if (speed > MAX_SPEED) {
		speed -= 30;
		KillTimer(hwnd, 1);
		SetTimer(hwnd, 1, speed, &TimerProc);
	}
}

int checkBody(HWND hwnd) {
	if (find(snake_body.begin(), snake_body.end(), snake_head) != snake_body.end()) {
		return 1;
	}
	return 0;
}

int checkWall(HWND hwnd) {
	if (snake_head.first == 0 || snake_head.first == HEIGHT_LINE - 1 ||
		snake_head.second == 0 || snake_head.second == WIDTH_LINE - 1) {
		return 1;
	}
	return 0;
}

void gameOver(HWND hwnd) {
	KillTimer(hwnd, 1);
	MessageBox(hwnd, L"Game Over", L"게임 오버", MB_OK);
	memset(screen, 0, sizeof(screen));
	snake_head = { HEIGHT_LINE / 2 - 1, WIDTH_LINE / 2 - 1 };
	snake_body.clear();
	apple = { HEIGHT_LINE / 2 - 1, WIDTH_LINE / 2 + 2 };
	direction = RIGHT;
	start = 0;
	score = 0;
	speed = START_SPEED;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	
	if (umsg == WM_PAINT) {
		drawMap(hwnd);
	}
	else if (umsg == WM_LBUTTONDOWN) {
		if (start == 0) {

			screen[snake_head.first][snake_head.second] = HEAD;
			screen[apple.first][apple.second] = APPLE;
			SetTimer(hwnd, 1, speed, &TimerProc);
		}
		start = 1;
		InvalidateRect(hwnd, NULL, TRUE);
		
	}
	else if (umsg == WM_DESTROY) {
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
	}
	else if (umsg == WM_CHAR) {
		if (wparam == VK_LEFT || wparam == 'a') {

			direction = LEFT;
		}
		if (wparam == VK_RIGHT || wparam == 'd') {

			direction = RIGHT;
		}
		if (wparam == VK_UP || wparam == 'w') {

			direction = UP;
		}
		if (wparam == VK_DOWN || wparam == 's') {

			direction = DOWN;
		}
		//cout << umsg << " " << wparam << " " << lparam << endl;
		//InvalidateRect(hwnd, NULL, TRUE);
		//drawMap(hwnd);

	}
	



	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

VOID CALLBACK TimerProc(HWND hwnd, UINT, UINT_PTR idEvent, DWORD)
{
	if (start == 1) {
		moveSnake(hwnd);
		checkApple(hwnd);

		if (checkBody(hwnd) == 1 || checkWall(hwnd) == 1) {
			
			InvalidateRect(hwnd, NULL, TRUE);
			gameOver(hwnd);
		}

		
		InvalidateRect(hwnd, NULL, TRUE);
	}

}

int WINAPI main(HINSTANCE hIns, HINSTANCE hPrevIns, LPSTR lpCmdLine, int nCmdShow) {

	WNDCLASS wc;
	wchar_t class_name[] = L"snake";

	HBRUSH background = CreateSolidBrush(RGB(0, 0, 0));

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
		2 * WHITE_SPACE + WIDTH_LINE * INTERVAL, 2.5 * WHITE_SPACE + HEIGHT_LINE * INTERVAL,
		NULL, NULL, hIns, NULL);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	
	
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DeleteObject(background);

	return (int)msg.wParam;

}