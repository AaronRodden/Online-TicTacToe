#include "Piece.h"
#include <iostream>

Piece::Piece() {

}

Piece::Piece(int x, int y, int playerID) : position(x, y), velocity(0, 0), circle(40, 100), triangle(60, 3), selectedFlag(false) {
	if (playerID == 0) {
		circle.setOrigin(sf::Vector2f(50, 50));
		targetPiece = circle;
	}
	if (playerID == 1) {
		triangle.setOrigin(sf::Vector2f(50, 50));
		targetPiece = triangle;
	}
}

bool Piece::isNonActive() {
	if (position.x == NULL) {
		return true;
	}
	return false;
}

void Piece::update() {
	position.x += velocity.x;
	position.y += velocity.y;

	//std::cout << "Position x: " << position.x << std::endl;
	//std::cout << "Position y: " << position.y << std::endl;

	targetPiece.setPosition(position);

	if (selectedFlag == true) {
		targetPiece.setFillColor(sf::Color::Yellow);
	}
	else {
		targetPiece.setFillColor(sf::Color::White);
	}
}

void Piece::selected() {
	selectedFlag = true;
}

void Piece::draw(sf::RenderWindow& window) {
	window.draw(targetPiece);
}

void Piece::setVelocity(sf::Vector2f vec) {
	velocity = vec;
}

void Piece::setPosition(sf::Vector2f vec) {
	position = vec;
}

sf::CircleShape Piece::getPiece() {
	return targetPiece;
}