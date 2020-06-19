#include "GameState.h"
#include <iostream>

//Empty Constructor as of now
GameState::GameState() {

}

sf::RectangleShape* GameState::getBoard() {
	sf::RectangleShape* boardGraphics = new sf::RectangleShape[4];
	sf::RectangleShape line(sf::Vector2f(5, 500));
	line.setPosition(260.f, 60.f);
	boardGraphics[0] = line;
	line.setPosition(530.f, 60.f);
	boardGraphics[1] = line;
	line.rotate(90);
	line.setPosition(650.f, 200.f);
	boardGraphics[2] = line;
	line.setPosition(650.f, 400.f);
	boardGraphics[3] = line;
	return boardGraphics;
}

void GameState::checkState() {
	// row victories
	if ((board[0][0] == 0 && board[0][1] == 0 && board[0][2] == 0)
		|| (board[0][0] == 1 && board[0][1] == 1 && board[0][2] == 1)) {
		victory = board[0][0];
		//std::cout << "Victory for player " << victory << std::endl;
	}
	else if ((board[1][0] == 0 && board[1][1] == 0 && board[1][2] == 0)
		|| (board[1][0] == 1 && board[1][1] == 1 && board[1][2] == 1)) {
		victory = board[1][0];
		//std::cout << "Victory for player " << victory << std::endl;
	}
	else if ((board[2][0] == 0 && board[2][1] == 0 && board[2][2] == 0)
		|| (board[2][0] == 1 && board[2][1] == 1 && board[2][2] == 1)) {
		victory = board[2][0];
		//std::cout << "Victory for player " << victory << std::endl;
	}
	// col victories
	else if ((board[0][0] == 0 && board[1][0] == 0 && board[2][0] == 0)
		|| (board[0][0] == 1 && board[1][0] == 1 && board[2][0] == 1)) {
		victory = board[0][0];
		//std::cout << "Victory for player " << victory << std::endl;
	}
	else if ((board[0][1] == 0 && board[1][1] == 0 && board[2][1] == 0)
		|| (board[0][1] == 1 && board[1][1] == 1 && board[2][1] == 1)) {
		victory = board[0][1];
		//std::cout << "Victory for player " << victory << std::endl;
	}
	else if ((board[0][2] == 0 && board[0][2] == 0 && board[2][2] == 0)
		|| (board[0][2] == 1 && board[0][2] == 1 && board[2][2] == 1)) {
		victory = board[0][2];
		//std::cout << "Victory for player " << victory << std::endl;
	}
	// diagonal victories
	else if ((board[0][0] == 0 && board[1][1] == 0 && board[2][2] == 0)
		|| (board[0][0] == 1 && board[1][1] == 1 && board[2][2] == 1)) {
		victory = board[0][0];
		//std::cout << "Victory for player " << victory << std::endl;
	}
	else if ((board[2][0] == 0 && board[1][1] == 0 && board[0][2] == 0)
		|| (board[2][0] == 1 && board[1][1] == 1 && board[0][2] == 1)) {
		victory = board[2][0];
		//std::cout << "Victory for player " << victory << std::endl;
	}
}

void GameState::updateBoard(int x, int y, int playerID) {
	//When a piece is placed in a certain place on the board, update respective cell
	// x, y mouse coordinants --> respective cell

	// left cells
	if (x < 260 && y < 180) {
		//std::cout << "Clicked on top left" << std::endl;
		board[0][0] = playerID;
	}
	else if (x < 260 && y > 180 && y < 390) {
		//std::cout << "Clicked on left center" << std::endl;
		board[1][0] = playerID;
	}
	else if (x < 260 && y > 390 && y < 620) {
		//std::cout << "Clicked on bottom left" << std::endl;
		board[2][0] = playerID;
	}

	//middle cells
	else if (x > 260 && x < 530 && y < 180) {
		//std::cout << "Clicked on top middle" << std::endl;
		board[0][1] = playerID;
	}
	else if (x > 260 && x < 530 && y > 180 && y < 390) {
		//std::cout << "Clicked in dead center" << std::endl;
		board[1][1] = playerID;
	}
	else if (x > 260 && x < 530 && y > 390 && y < 620) {
		//std::cout << "Clicked on bottom middle" << std::endl;
		board[2][1] = playerID;
	}
	
	//right cells
	else if (x > 530 && x < 700 && y < 180) {
		//std::cout << "Clicked on top right" << std::endl;
		board[0][2] = playerID;
	}
	else if (x > 530 && x < 700 && y > 180 && y < 390) {
		//std::cout << "Clicked in right center" << std::endl;
		board[1][2] = playerID;
	}
	else if (x > 530 && x < 700 && y > 390 && y < 620) {
		//std::cout << "Clicked on bottom right" << std::endl;
		board[2][2] = playerID;
	}
	else if (x > 700) {
		//std::cout << "Clicked off board" << std::endl;
	}
	else if (y > 640) {
		//std::cout << "Clicked off board" << std::endl;
	}

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			std::cout << board[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}