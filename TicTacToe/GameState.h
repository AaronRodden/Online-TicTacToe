#pragma once
#include <SFML/Graphics.hpp>

class GameState {
public:
	GameState();
	int victory;
	int board[3][3];

	sf::RectangleShape* getBoard();
	void updateBoard(int x, int y, int playerID);
	void checkState();

private:
};