#pragma once
#include <SFML/Graphics.hpp>

class Piece {
public:
	Piece();
	Piece(int x, int y, int playerID);
	bool isNonActive();
	void update();
	void selected();
	void draw(sf::RenderWindow& window);
	void setVelocity(sf::Vector2f vec);
	void setPosition(sf::Vector2f vec);

	sf::CircleShape getPiece();

	bool selectedFlag;

private:
	sf::CircleShape circle;
	sf::CircleShape triangle;
	sf::CircleShape targetPiece;
	sf::Vector2f position;
	sf::Vector2f velocity;
};