#pragma once
#include <SFML/Graphics.hpp>
#include "Piece.h"

class PieceSpawner {
public:
	PieceSpawner(int ID);
	int playerID;
	sf::FloatRect globalBounds;

	void update();
	void draw(sf::RenderWindow& window);
	Piece spawnPiece();

private:
	sf::CircleShape circle;
	sf::CircleShape triangle;

};