#include <iostream>
#include <string> 
#include <cstdlib> 
#include <ctime>
#include <string.h>
#include <stdio.h>
#include <deque>
#include <algorithm>
#include <conio.h>
using namespace std;



class Snake {
public:
	void setting();
	void drawing();
	void moveSnake();
	void checkEvent();
	void setSnakeBody(pair <float, float> move);
	bool isLive();
	void loadGameOver(int type);
	void init();
	Snake() : map_width(20), map_height(10), m_score(0), m_move_score(0), live(true), m_snake_length(1) {};

private:
	pair<float, float> apple;
	float map_width;
	float map_height;
	float m_score;
	float m_move_score;
	pair<float, float> snake;
	bool live;
	deque<pair<float, float>> snake_body;
	float m_snake_length;
};

void Snake::setting() {
	float mw = map_width / 2;
	float mh = map_height / 2;
	apple.first = mw + 2;
	apple.second = mh;
	snake.first = mw;
	snake.second = mh;

}



void Snake::drawing() {
	system("cls");
	for (int i = 0; i < map_width + 1; i++) {
		cout << "w";
	}
	cout << "\n";

	for (int i = 1; i < map_height + 1; i++) {
		for (int j = 0; j < map_width + 1; j++) {
			if (j == 0 || j == map_width) {
				cout << "w";
			}
			else if (snake == pair<float, float> (j, i)) {
				cout << "@";
			}
			else if (find(snake_body.begin(), snake_body.end(), pair<float, float>(j, i)) != snake_body.end()) {
				cout << "O";
			}
			else if (apple == pair<float, float>(j, i)) {
				cout << "A";
			}
			else {
				cout << " ";
			}
		}
		cout << "\n";
	}

	for (int i = 0; i < map_width + 1; i++) {
		cout << "w";
	}
	cout << "\n\n\n";
	cout << "score : " << m_score << endl;
	cout << "\nmovement : " << m_move_score << endl;
	cout << "\nsnake head ('@') : (" << snake.first << ", " << snake.second << ")\n";
	cout << "\napple ('A') : (" << apple.first << ", " << apple.second << ")\n";
	cout << "\n a : left, d : right, w : up, s : down \n";
	cout << "\nif you want to end this game, press 'x' \n\n";
}

void Snake::moveSnake() {
	//if (_kbhit()) {
		switch (_getch()) {
		case 'a':
			
			snake.first -= 1.0f;
			m_move_score++;
			break;
		case 's':
			
			snake.second += 1.0f;
			m_move_score++;
			break;
		case 'w':
			
			snake.second -= 1.0f;
			m_move_score++;
			break;
		case 'd':
			
			snake.first += 1.0f;
			m_move_score++;
			break;
		case 'x':
			live = false;
			loadGameOver(2);
			break;
		default:
			break;
		}
	//}
}


void Snake::checkEvent() {
	// 벽 부딪힘 체크
	if (snake.second == 0 || snake.second == map_height + 1 || snake.first == 0 || snake.first == map_width) {
		loadGameOver(0);
	}
	// 몸통과 부딪혔는지 체크
	else if (find(snake_body.begin(), snake_body.end(), snake) != snake_body.end()) {
		loadGameOver(1);
	}
	// 사과 먹었는지 체크
	else if (snake == apple) {
		
		apple.first = rand() % ((int)map_width - 1) + 1;
		apple.second = rand() % ((int)map_height - 1) + 1;
		// 사과가 몸통에 생기지 않도록 
		while (snake == apple || find(snake_body.begin(), snake_body.end(), apple) != snake_body.end()) {
			apple.first = rand() % ((int)map_width - 1) + 1;
			apple.second = rand() % ((int)map_height - 1) + 1;
		}
		m_snake_length++;
		setSnakeBody(snake);
		m_score += 10;
	} 
	else {
		setSnakeBody(snake);
	}
	
}


void Snake::setSnakeBody(pair<float, float> move) {
	
	snake_body.push_back(move);

	while (!snake_body.empty() && snake_body.size() > m_snake_length) {
		snake_body.pop_front();
	}

}

bool Snake::isLive() {
	if (live == false) {
		return false;
	}
	return true;
}

void Snake::loadGameOver(int type) {
	live = false;
	system("cls");
	cout << "\n\n \t\t\tGame Over \n\n";
	switch (type) {
	case 0:
		cout << "\t\t\tkissed the wall \n\n";
		break;
	case 1:
		cout << "\t\t\tbitten snake body \n\n";
		break;
	case 2:
		cout << "\t\t\tpressed x \n\n";
		break;
	default:
		break;
	}
}
void Snake::init() {
	srand(time(0));
}
int main() {
	Snake snake;
	snake.init();
	snake.setting();
	

	while (snake.isLive()) {
		
		snake.drawing();
		snake.moveSnake();
		snake.checkEvent();

		
	}


	return 0;
}