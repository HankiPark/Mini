#include <iostream>
#include <string> 
#include <cstdlib> 
#include <ctime>
#include <string.h>
#include <stdio.h>

using namespace std;


class PlayerData {
public:
	int getMoney();
	void setMoney();
	void updateMoney(const bool result);
	void setBettingMoney();
	void setGuessNumber();
	bool isWin();
	int getGuessNumber();
	void init();
	PlayerData() :m_amount(0), m_betting(0), m_guess_number(-1) {}

private:
	float m_amount;
	float m_betting;
	int m_guess_number;
};

int PlayerData::getMoney() {
	return m_amount;
}

void PlayerData::setMoney() {
	do {
		cout << "\n Enter your init money : ";
		cin >> m_amount;
		if (m_amount < 0.01f) {
			cout << "\n Please enter valid init money to play the game!";
		}
	} while (m_amount < 0.01f);
}

void PlayerData::updateMoney(const bool result) {
	if (result) {
		cout << "\nCongratulate your win!! you get 3x your bet!! \n";
		m_amount += m_betting * 3;
	}
	else {
		cout << "\nFailure! you lose the amount you bet \n";
		m_amount -= m_betting;
	}
}

void PlayerData::setBettingMoney() {
	do {
		cout << "\n Enter your money to bet : ";
		cin >> m_betting;
		if (m_betting <= 0) {
			cout << "\n Bet the money more than zero \n";
		}
		if (m_amount < m_betting) {
			cout << "\n Your cash in hand :" << m_amount;
			cout << "\n Your betting money is more than your cash\n";
		}
		
		
	} while (m_betting <= 0 || m_amount < m_betting);
}

void PlayerData::setGuessNumber() {
	do {
		cout << "\n Guess your number to get money! range(1 to 10) : ";
		cin >> m_guess_number;
		if (m_guess_number <= 0 || m_guess_number > 10) {
			cout << "Wrong : Number range is between 1 to 10\n";
		}
	} while (m_guess_number <= 0 || m_guess_number > 10);
}

bool PlayerData::isWin() {
	int ran = rand() % 10 + 1;
	return m_guess_number == ran ? true : false;
}

int PlayerData::getGuessNumber() {
	return m_guess_number;
}

void PlayerData::init() {
	srand(time(0));
}


int main() {
	class PlayerData player;
	char cont;
	player.init();

	player.setMoney();

	do {

		cout << "\nYour money : " << player.getMoney() << endl;

		player.setBettingMoney();

		player.setGuessNumber();

		const bool result = player.isWin();

		player.updateMoney(result);

		if (player.getMoney() <= 0.01f) {
			cout << "\nYou have no money ! bye bye. \n";
			break;
		}
		cout << "\nDo you want to play again ? (y/n)";
		cin >> cont;

	} while (cont == 'Y' || cont == 'y');

	cout << "\n GAME OVER \n";
	cout << "\nYour money : " << player.getMoney() << endl;


	return 0;
}