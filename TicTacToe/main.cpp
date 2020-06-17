#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

//TEST these are for client / server debugging
#include <chrono>
#include <thread>

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")


#include "Piece.h"
#include "PieceSpawner.h"
#include "GameState.h"

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 1024

int WINDOW_WIDTH = 1000;
int WINDOW_HEIGHT = 800;

// TODO: I think this class is going to become the "Client" class
// Moves will be made from here then then get sent to the GameState "Server" class
// GameState then respons and sends information back to all the "clients"

// TODO: Game will recieve the argument for wheter it is client or server
void game() {

	// TODO: Proper code ran here whether you are client or server

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");

	// Declare and load a font
	sf::Font font;
	font.loadFromFile("fonts/arial.ttf");

	GameState runningState;

	//We may need to make a player class that has all this info
	int playerID = 0;
	PieceSpawner spawn(playerID);

	//TODO: Should we even have any of the for loop checks for this given that we are using one at all times?
	std::vector<Piece> activePieces;
	std::vector<Piece> placedPieces;

	// TODO: All this event stuff will then add to an event queue which will then be sent over network.
	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			// if piece has been selected && you click somewhere other then the piece itself
			for (int i = 0; i < activePieces.size(); i++)
			{
				if (activePieces[i].selectedFlag == true && !(activePieces[i].getPiece().getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))) {
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						sf::Vector2i vec = sf::Mouse::getPosition(window);
						std::cout << vec.x << " " << vec.y << std::endl;
						activePieces[i].setPosition(sf::Vector2f(vec.x, vec.y));
						activePieces[i].selectedFlag = false;
						placedPieces.push_back(activePieces[i]);
						activePieces.erase(activePieces.begin());
						runningState.updateBoard(event.mouseButton.x, event.mouseButton.y, playerID);
					}
				}
			}

			// Click handler
			if (event.type == sf::Event::MouseButtonPressed)
			{
				//if left button pressed
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					// if you click the spawner
					if (spawn.globalBounds.contains(event.mouseButton.x, event.mouseButton.y))
					{
						if (activePieces.size() < 1) {
							activePieces.push_back(spawn.spawnPiece());
							activePieces[0].selected();
							std::cout << "Piece spawned and selected" << std::endl;
						}
					}
				}
			}

			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		// Every couple seconds check for messages here, update respective game objects accordingly

		// GAME UPDATES
		for (int i = 0; i < activePieces.size(); i++)
		{
			activePieces[i].update();
		}

		for (int i = 0; i < placedPieces.size(); i++)
		{
			placedPieces[i].update();
		}

		//RENDERING
		window.clear();

		//1. Draw the hex
		sf::RectangleShape* board = runningState.getBoard();
		for (int i = 0; i < sizeof(board); i++) {
			window.draw(board[i]);
		}

		//2. Draw Piece Spawners
		spawn.draw(window);

		//3. Draw all active pieces
		for (int i = 0; i < activePieces.size(); i++)
		{
			activePieces[i].draw(window);
		}

		//4. Draw all placed pieces
		for (int i = 0; i < placedPieces.size(); i++)
		{
			placedPieces[i].draw(window);
		}

		//5. If there is a victory, draw victory text 
		if (runningState.victory == 0 || runningState.victory == 1) {
			sf::Text text("victory for player " + runningState.victory, font);
			text.setCharacterSize(100);
			text.setStyle(sf::Text::Bold);
			text.setFillColor(sf::Color::Red);
			text.setPosition(200, 300);
			window.draw(text);
		}

		window.display();
	}
}

int server() {
	WSADATA wsaData;

	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	std::cout << "Initalized winsock" << std::endl;

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	std::cout << "Resolved local address and port" << std::endl;

	SOCKET ListenSocket = INVALID_SOCKET;
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	std::cout << "Setup TCP listening socket" << std::endl;

	freeaddrinfo(result);

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	SOCKET ClientSocket;
	ClientSocket = INVALID_SOCKET;

	std::cout << "Listening for client socket..." << std::endl;

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	std::cout << "Accepted client socket" << std::endl;

	char recvbuf[DEFAULT_BUFLEN];
	int iSendResult;
	int recvbuflen = DEFAULT_BUFLEN;

	//TODO: This will be the part that gets called in between game rendering
	// Just keep all the sockets then pass them into functions that do the
	// below functionallity. Then the functions return the messages

	// Receive until the peer shuts down the connection
	do {

		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		std::cout << "iResult: " << iResult << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);
			std::cout << recvbuf << std::endl;

			// Echo the buffer back to the sender
			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed: %d\n", WSAGetLastError());
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0) {
			printf("Connection closing...\n");
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		else {
			printf("recv failed: %d\n", WSAGetLastError());
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}

	} while (iResult > 0);

	return 0;
}

int client() {

	int iResult;
	WSADATA wsaData;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cout << "WSAStartup failed: " << iResult << std::endl;
		return 1;
	}

	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	//int iResult;
	char const *hardCodeIP = "192.168.2.141";

	// Resolve the server address and port
	iResult = getaddrinfo(hardCodeIP, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		WSACleanup();
		return 1;
	}

	SOCKET ConnectSocket = INVALID_SOCKET;
	// Attempt to connect to the first address returned by
	// the call to getaddrinfo
	ptr = result;

	// Create a SOCKET for connecting to server
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
		ptr->ai_protocol);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Connect to server.
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	// Should really try the next address returned by getaddrinfo
	// if the connect call failed
	// But for this simple example we just free the resources
	// returned by getaddrinfo and print an error message

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	int recvbuflen = DEFAULT_BUFLEN;

	char userInput[100];
	const char *sendbuf = "this is a test";

	std::cin >> userInput;

	char recvbuf[DEFAULT_BUFLEN];

	// Send an initial buffer
	iResult = send(ConnectSocket, userInput, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);

	// shutdown the connection for sending since no more data will be sent
	// the client can still use the ConnectSocket for receiving data
	//iResult = shutdown(ConnectSocket, SD_SEND);
	//if (iResult == SOCKET_ERROR) {
	//	printf("shutdown failed: %d\n", WSAGetLastError());
	//	closesocket(ConnectSocket);
	//	WSACleanup();
	//	return 1;
	//}

	// Receive data until the server closes the connection
	do {
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);
			std::cout << recvbuf << std::endl;
			std::cin >> userInput;
			iResult = send(ConnectSocket, userInput, (int)strlen(sendbuf), 0);
			if (iResult == SOCKET_ERROR) {
				printf("send failed: %d\n", WSAGetLastError());
				closesocket(ConnectSocket);
				WSACleanup();
				return 1;
			}
			printf("Bytes Sent: %ld\n", iResult);
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	} while (iResult > 0);

	return 0;
}

int main()
{
	//game();
	int x;
	std::cout << "0 for server, 1 for client" << std::endl;
	std::cin >> x;
	std::cout << "You entered: " << x << std::endl;

	if (x == 0) {
		server();
	}
	else {
		client();
	}
	return 0;
}
