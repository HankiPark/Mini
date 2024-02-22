#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
	char user_input;
	int computer;
	srand(time(0));
	
	while (true) {
		computer = rand() % 3;
		std::cout << "Enter 'r' for rock, 'p' for paper, 's' for scissors, 'q' to quit.\n";
		std::cin >> user_input;
		//std::cin.ignore();
		//Sleep(1000);
		if (user_input == 'q') {
			break;
		}
		else if (user_input == 'r') {
			user_input = 0;
		}
		else if (user_input == 'p') {
			user_input = 1;
		}
		else if (user_input == 's') {
			user_input = 2;
		}
		else {
			user_input = 3;
		}




		if (user_input == 3) {
			std::cout << "wrong input!! \n";
		} else if (user_input == computer) {
			std::cout << "It's a Tie! \n";
		}
		else if ((user_input + 1) % 3 == computer) {
			std::cout << "You lose! \n";
		}
		else {
			std::cout << "You Win! \n";
		}
	}
	return 0;
}