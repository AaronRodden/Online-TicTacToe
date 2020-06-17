#include "PieceSpawner.h"

PieceSpawner::PieceSpawner(int ID) : playerID(ID), circle(40, 100), triangle(60, 3), globalBounds() {
	circle.setOrigin(sf::Vector2f(50, 50));
	triangle.setOrigin(sf::Vector2f(50, 50));
	circle.setPosition(800, 150);
	triangle.setPosition(800, 150);

	globalBounds = circle.getGlobalBounds();
}

void PieceSpawner::draw(sf::RenderWindow& window) {
	if (playerID == 0) {
		window.draw(circle);
	}
	else {
		window.draw(triangle);
	}
}

void PieceSpawner::update() {

}

Piece PieceSpawner::spawnPiece(int pID) {
	// make certain pieces based of player id
	if (pID == 0) {
		Piece circlePiece(800, 150, 0);
		return circlePiece;
	}
	else {
		Piece trianglePiece(800, 150, 1);
		return trianglePiece;
	}
}