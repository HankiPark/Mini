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

#define WIDTH_LINE		19
#define HEIGHT_LINE		19

#define WHITE_SPACE 50

#define INTERVAL	30
#define RADIUS		(INTERVAL / 2)

#define POS(x)		(WHITE_SPACE + (x) * INTERVAL)

#define FOUR_SCORE	7
#define WIN_SCORE	100

#define PII			pair<int, int>


#define BLACK		1
#define WHITE		2
#define BLACK_WIN	3
#define WHITE_WIN	4

unsigned int dol[HEIGHT_LINE][WIDTH_LINE];
unsigned int order = BLACK;
unsigned int game_mode = 0;
vector<PII> completed_dol_loc;
//pair<int, int> computer_dol;

void drawMap(HWND hand);
void checkNear(int height, int width, vector<vector<int>>& visited);
PII whiteAI();
PII buttonDown(HWND hwnd, int height, int width);
int checkLineValue(PII loc);
LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
int WINAPI main(HINSTANCE hIns, HINSTANCE hPrevIns, LPSTR lpCmdLine, int nCmdShow);

//좌표에 대해 (x, y)는 x는 세로, y는 가로로 지정 dol[x][y];

void drawMap(HWND hwnd) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	int check_line = 0;

	for (int i = 0; i < WIDTH_LINE; i++) {
		MoveToEx(hdc, POS(i), POS(0), NULL);
		LineTo(hdc, POS(i), POS(HEIGHT_LINE - 1));
	}

	for (int i = 0; i < HEIGHT_LINE; i++) {
		MoveToEx(hdc, POS(0), POS(i), NULL);
		LineTo(hdc, POS(WIDTH_LINE - 1), POS(i));
	}

	for (int i = 0; i < HEIGHT_LINE; i++) {
		for (int j = 0; j < WIDTH_LINE; j++) {
			if (dol[i][j] > 0) {
				if (dol[i][j] == BLACK) {
					SelectObject(hdc, GetStockObject(BLACK_BRUSH));
					Ellipse(hdc, POS(j) - RADIUS, POS(i) - RADIUS, POS(j) + RADIUS, POS(i) + RADIUS);
				}
				else if (dol[i][j] == WHITE) {
					SelectObject(hdc, GetStockObject(WHITE_BRUSH));
					Ellipse(hdc, POS(j) - RADIUS, POS(i) - RADIUS, POS(j) + RADIUS, POS(i) + RADIUS);
				}
				else if (dol[i][j] == BLACK_WIN) {
					check_line = 1;
					SelectObject(hdc, GetStockObject(BLACK_BRUSH));
					Ellipse(hdc, POS(j) - RADIUS * 1.2, POS(i) - RADIUS * 1.2, POS(j) + RADIUS * 1.2, POS(i) + RADIUS * 1.2);
				}
				else if (dol[i][j] == WHITE_WIN) {
					check_line = 1;
					SelectObject(hdc, GetStockObject(WHITE_BRUSH));
					Ellipse(hdc, POS(j) - RADIUS * 1.2, POS(i) - RADIUS * 1.2, POS(j) + RADIUS * 1.2, POS(i) + RADIUS * 1.2);
				}
				
			}
		}
	}


	if (check_line == 1) {
		HPEN hPen = CreatePen(PS_SOLID, 5, RGB(237, 28, 36));
		HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

		MoveToEx(hdc, POS(completed_dol_loc[0].second), POS(completed_dol_loc[0].first),  NULL);
		LineTo(hdc, POS(completed_dol_loc.back().second),  POS(completed_dol_loc.back().first));

		SelectObject(hdc, hOldPen);
		DeleteObject(hPen);
		
	}

	EndPaint(hwnd, &ps);
}

void checkNear(int height, int width, vector<vector<int>>& visited) {
	int dh[8] = {1, 1, 1, 0, 0, -1, -1, -1};
	int dw[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
	int max_value = 0;
	for (int i = 0; i < 8; i++) {
		int new_height = height + dh[i];
		int new_width = width + dw[i];
		if (new_height < 0 || new_width >= WIDTH_LINE || new_width < 0 || new_height >= HEIGHT_LINE) {
			continue;
		}
		// 돌이 들어갔을 경우 값 계산을 위함
		if (dol[new_height][new_width] == 0) {
			dol[new_height][new_width] = order;
			int t = checkLineValue(PII(new_height, new_width));
			dol[new_height][new_width] = 0;
			// 자기돌에 가중치 부여
			if (order == WHITE) {
				t = t * 1.5;
			}
			visited[new_height][new_width] = max(visited[new_height][new_width], t);
		}

	}
	

}

//현재 놓여있는 돌들의 점수를 계산해서 위치 파악하기
PII whiteAI() {
	vector<vector<int>> visited(HEIGHT_LINE, vector<int> (WIDTH_LINE, 0));
	for (int i = 0; i < HEIGHT_LINE; i++) {
		for (int j = 0; j < WIDTH_LINE; j++) {
			if (dol[i][j] == 0) {
				continue;
			}
			checkNear(i, j, visited);
			if (order == BLACK) {
				order = WHITE;
			}
			else {
				order = BLACK;
			}
			checkNear(i, j, visited);
			if (order == BLACK) {
				order = WHITE;
			}
			else {
				order = BLACK;
			}
		}
	}
	int max_value = 0;
	vector<PII> marking;

	for (int i = 0; i < HEIGHT_LINE; i++) {
		for (int j = 0; j < WIDTH_LINE; j++) {
			// 점수 체크용 cout
			/*
			if (visited[i][j] > 9) {
				cout << visited[i][j] << "  ";
			}
			else if (visited[i][j] > 99) {
				cout << visited[i][j] << " ";
			}
			else {
				cout << visited[i][j] << "   ";
			}
			*/
			
			if (visited[i][j] > max_value) {
				marking.clear();
				marking.push_back(PII(i, j));
				max_value = visited[i][j];
			}
			else if (visited[i][j] == max_value) {
				marking.push_back(PII(i, j));
			}
		}
		cout << endl;
	}

	int markingsize = marking.size();
	int choose = rand() % markingsize;

	return { marking[choose].first, marking[choose].second };
}

PII buttonDown(HWND hwnd, int height, int width) {
	if (width > POS(0) - RADIUS && height > POS(0) - RADIUS &&
		width < POS(WIDTH_LINE - 1) + RADIUS && height < POS(HEIGHT_LINE - 1) + RADIUS) {
		int dol_width = (width - WHITE_SPACE + RADIUS) / INTERVAL;
		int dol_height = (height - WHITE_SPACE + RADIUS) / INTERVAL;
		// 1 vs 1 
		if (game_mode == 1) {
			if (dol[dol_height][dol_width] == 0) {
				dol[dol_height][dol_width] = order;
				if (order == BLACK) {
					order = WHITE;
				}
				else {
					order = BLACK;
				}
				InvalidateRect(hwnd, NULL, TRUE);

				return PII(dol_height, dol_width);
			}
		}
		// vs computer
		else {
			if (dol[dol_height][dol_width] == 0) {
				dol[dol_height][dol_width] = order;
				InvalidateRect(hwnd, NULL, TRUE);
				return PII (dol_height, dol_width);
			}
		}

	}

	return PII(-1, -1);
}

// 게임오버를 검사하는 함수에서, 컴퓨터가 알을 놓기위한 점수 계산방식으로 바꿈
int checkLineValue(PII loc) {
	int height = loc.first;
	int width = loc.second;
	int dol_color = dol[height][width];
	int check_width = 1;
	int check_height = 1;
	int check_downward_diagonal = 1;
	int check_upward_diagonal = 1;
	int count = 0;
	int max_value = 0;
	vector<PII> temp;

	temp.push_back(loc);
	for (int i = width - 1; i >= 0; i--) {
		if (count++ > 5) {
			break;
		}
		if (dol[height][i] == dol_color) {
			check_width++;
			temp.push_back({ height, i });
		}
		else {
			break;
		}
	}
	
	for (int i = width + 1; i <= WIDTH_LINE; i++) {
		if (count++ > 5) {
			break;
		}
		if (dol[height][i] == dol_color) {
			check_width++;
			temp.push_back({ height, i });
		}
		else {
			break;
		}
	}
	
	switch (check_width) {
	case 0:
		break;
	case 1:
		max_value += 1;
		break;
	case 2:
		max_value += 3;
		break;
	case 3:
		max_value += 5;
		break;
	case 4:
		max_value += 20;
		break;
	default:
		max_value += 100;
		completed_dol_loc.clear();
		completed_dol_loc.insert(completed_dol_loc.begin(), temp.begin(), temp.end());
		break;
	}

	count = 0;
	temp.clear();
	temp.push_back(loc);
	for (int i = height - 1; i >= 0; i--) {
		if (count++ > 5) {
			break;
		}
		if (dol[i][width] == dol_color) {
			check_height++;
			temp.push_back({ i, width });
		}
		else {
			break;
		}
	}
	
	for (int i = height + 1; i <= HEIGHT_LINE; i++) {
		if (count++ > 5) {
			break;
		}
		if (dol[i][width] == dol_color) {
			check_height++;
			temp.push_back({ i, width });
		}
		else {
			break;
		}
	}
	
	switch (check_height) {
	case 0:
		break;
	case 1:
		max_value += 1;
		break;
	case 2:
		max_value += 3;
		break;
	case 3:
		max_value += 5;
		break;
	case 4:
		max_value += 20;
		break;
	default:
		max_value += 100;
		completed_dol_loc.clear();
		completed_dol_loc.insert(completed_dol_loc.begin(), temp.begin(), temp.end());
		break;
	}

	count = 0;
	temp.clear();
	temp.push_back(loc);

	int temp_width = width - 1;

	for (int i = height - 1; i >= 0; i--) {
		if (temp_width < 0 || count++ > 5) {
			break;
		}
		if (dol[i][temp_width--] == dol_color) {
			check_downward_diagonal++;
			temp.push_back({ i, temp_width + 1 });
		}
		else {
			break;
		}
	}
	
	temp_width = width + 1;

	for (int i = height + 1; i <= HEIGHT_LINE; i++) {
		if (temp_width > WIDTH_LINE || count++ > 5) {
			break;
		}
		if (dol[i][temp_width++] == dol_color) {
			check_downward_diagonal++;
			temp.push_back({ i, temp_width - 1 });
		}
		else {
			break;
		}
	}
	
	switch (check_downward_diagonal) {
	case 0:
		break;
	case 1:
		max_value += 1;
		break;
	case 2:
		max_value += 3;
		break;
	case 3:
		max_value += 5;
		break;
	case 4:
		max_value += 20;
		break;
	default:
		max_value += 100;
		completed_dol_loc.clear();
		completed_dol_loc.insert(completed_dol_loc.begin(), temp.begin(), temp.end());
		break;
	}

	count = 0;
	temp.clear();
	temp.push_back(loc);

	temp_width = width + 1;
	for (int i = height - 1; i >= 0; i--) {
		if (temp_width > WIDTH_LINE || count++ > 5) {
			break;
		}
		if (dol[i][temp_width++] == dol_color) {
			check_upward_diagonal++;
			temp.push_back({ i, temp_width - 1 });
		}
		else {
			break;
		}
	}
	
	temp_width = width - 1;
	for (int i = height + 1; i <= HEIGHT_LINE; i++) {
		if (temp_width < 0 || count++ > 5) {
			break;
		}
		if (dol[i][temp_width--] == dol_color) {
			check_upward_diagonal++;
			temp.push_back({ i, temp_width + 1 });
		}
		else {
			break;
		}
	}
	
	switch (check_upward_diagonal) {
	case 0:
		break;
	case 1:
		max_value += 1;
		break;
	case 2:
		max_value += 3;
		break;
	case 3:
		max_value += 5;
		break;
	case 4:
		max_value += 20;
		break;
	default:
		max_value += 100;
		completed_dol_loc.clear();
		completed_dol_loc.insert(completed_dol_loc.begin(), temp.begin(), temp.end());
		break;
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
	int check_width = 1;
	int check_height = 1;
	int check_downward_diagonal = 1;
	int check_upward_diagonal = 1;
	int count = 0;

	for (int i = x - 1; i >= 0; i--) {
		if (count++ > 5) {
			break;
		}
		if (dol[y][i] == dol_color) {
			check_width++;
		}
		else {
			break;
		}
	}

	for (int i = x + 1; i <= WIDTH_LINE; i++) {
		if (count++ > 5) {
			break;
		}
		if (dol[y][i] == dol_color) {
			check_width++;
		}
		else {
			break;
		}
	}

	if (check_width >= 5) {
		return dol_color;
	}

	count = 0;

	for (int i = y - 1; i >= 0; i--) {
		if (count++ > 5) {
			break;
		}
		if (dol[i][x] == dol_color) {
			check_height++;
		}
		else {
			break;
		}
	}

	for (int i = y + 1; i <= HEIGHT_LINE; i++) {
		if (count++ > 5) {
			break;
		}
		if (dol[i][x] == dol_color) {
			check_height++;
		}
		else {
			break;
		}
	}

	if (check_height >= 5) {
		return dol_color;
	}

	count = 0;

	int temp_x = x - 1;

	for (int i = y - 1; i >= 0; i--) {
		if (temp_x < 0 || count++ > 5) {
			break;
		}
		if (dol[i][temp_x--] == dol_color) {
			check_downward_diagonal++;
		}
		else {
			break;
		}
	}

	temp_x = x + 1;

	for (int i = y + 1; i <= HEIGHT_LINE; i++) {
		if (temp_x > WIDTH_LINE || count++ > 5) {
			break;
		}
		if (dol[i][temp_x++] == dol_color) {
			check_downward_diagonal++;
		}
		else {
			break;
		}
	}

	if (check_downward_diagonal >= 5) {
		return dol_color;
	}

	count = 0;

	temp_x = x + 1;
	for (int i = y - 1; i >= 0; i--) {
		if (temp_x > WIDTH_LINE || count++ > 5) {
			break;
		}
		if (dol[i][temp_x++] == dol_color) {
			check_upward_diagonal++;
		}
		else {
			break;
		}
	}
	temp_x = x - 1;
	for (int i = y + 1; i <= HEIGHT_LINE; i++) {
		if (temp_x < 0 || count++ > 5) {
			break;
		}
		if (dol[i][temp_x--] == dol_color) {
			check_upward_diagonal++;
		}
		else {
			break;
		}
	}

	if (check_upward_diagonal >= 5) {
		return dol_color;
	}

	count = 0;

	return 0;
}
*/

bool cmp(PII a, PII b) {
	if (a.first == b.first) {
		return a.second > b.second;
	} 
	return a.first > b.first;
}

void WinDolChange(HWND hwnd, int dol_color) {
	PAINTSTRUCT ps;
	sort(completed_dol_loc.begin(), completed_dol_loc.end(), cmp);
	for (PII loc : completed_dol_loc) {
		cout << loc.first << loc.second << endl;
		if (dol_color == BLACK) {
			dol[loc.first][loc.second] = BLACK_WIN;
		}
		else if (dol_color == WHITE) {
			dol[loc.first][loc.second] = WHITE_WIN;
		}
	}



	InvalidateRect(hwnd, NULL, TRUE);
	drawMap(hwnd);

	EndPaint(hwnd, &ps);
	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	
	if (umsg == WM_PAINT) {
		drawMap(hwnd);
	}
	else if (umsg == WM_LBUTTONDOWN) {
		
		PII loc = buttonDown(hwnd, (int)HIWORD(lparam), (int)LOWORD(lparam));
		if (loc.first == -1 && loc.second == -1) {
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
		// 1 vs 1
		if (game_mode == 1) {
			unsigned int value = checkLineValue(loc);

			// 순서는 돌을 놓는 순간 전환되기 때문에 order가 반대일 경우로 계산
			if (order == WHITE && value >= WIN_SCORE) {
				//cout << completed_dol_loc.size() << endl;
				WinDolChange(hwnd, BLACK);
				MessageBox(hwnd, L"흑돌승리", L"Black Win!", MB_OK);
				memset(dol, 0, sizeof(dol));
				order = BLACK;
				InvalidateRect(hwnd, NULL, TRUE);
			}
			else if (order == BLACK && value >= WIN_SCORE) {
				WinDolChange(hwnd, WHITE);
				MessageBox(hwnd, L"백돌승리", L"White Win!", MB_OK);
				memset(dol, 0, sizeof(dol));
				order = BLACK;
				InvalidateRect(hwnd, NULL, TRUE);
			}
		}
		// vs computer
		else {
			unsigned int value = checkLineValue(loc);
			// 컴퓨터가 돌을 놓는 것을 좀 더 자연스럽게 검은 돌 두고 화면 갱신
			InvalidateRect(hwnd, NULL, TRUE);
			drawMap(hwnd);
			if (value >= WIN_SCORE) {
				WinDolChange(hwnd, BLACK);
				MessageBox(hwnd, L"흑돌승리", L"Black Win!", MB_OK);
				memset(dol, 0, sizeof(dol));
				order = BLACK;
				InvalidateRect(hwnd, NULL, TRUE);
			}
			else {
				PII com = whiteAI();
				
				dol[com.first][com.second] = 2;
				order = WHITE;
				value = checkLineValue(com);
				order = BLACK;
				// 컴퓨터가 돌을 놓는 것을 좀 더 자연스럽게 흰 돌 두고 1초뒤 화면 갱신
				Sleep(1000);
				InvalidateRect(hwnd, NULL, TRUE);
				drawMap(hwnd);
				if (value >= WIN_SCORE) {
					WinDolChange(hwnd, WHITE);
					MessageBox(hwnd, L"백돌승리", L"Black Win!", MB_OK);
					memset(dol, 0, sizeof(dol));
					order = BLACK;
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
		2 * WHITE_SPACE + HEIGHT_LINE * INTERVAL, 2 * WHITE_SPACE + WIDTH_LINE * INTERVAL, 
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