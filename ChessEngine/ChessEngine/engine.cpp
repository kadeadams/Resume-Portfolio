#include "engine.h"


#include <iostream>
#include <string>
#include <vector>



ChessEngine::ChessEngine() {
	chessBoard = new Board;
}

ChessEngine::~ChessEngine() {

}

void ChessEngine::play() {
	std::cout << "Starting Game..." << std::endl << std::endl;
	chessBoard->printBoard();
	
	while (true) {
		Move playerMove = getPlay();
		if (playerMove.moveType == FAILED) {
			std::cout << "Quitting Game..." << std::endl;
			break;
		}
		if (playerMove.moveType == CAP && playerMove.capturedPiece == B_KING) {
			std::cout << "PLAYER WINS" << std::endl;
			break;
		}
		chessBoard->makeMove(playerMove);
		chessBoard->printBoard();
		std::cout << "---------------------------------------------------------" << std::endl;
		Move engineMove = chessBoard->negaMaxCaller();
		if (engineMove.moveType == CAP && engineMove.capturedPiece == W_KING) {
			std::cout << "AI WINS" << std::endl;
			break;
		}
		if (engineMove.moveType == FAILED) {
			std::cout << "PLAYER WINS" << std::endl;
			break;
		}
		chessBoard->makeMove(engineMove);
		chessBoard->printBoard();

	}
}

Move ChessEngine::getPlay() {

	std::cout << "Your Turn" << std::endl;
	std::string moveString;
	bool invalid = false;
	int firstSquare = 0;
	int secondSquare = 0;
	while (true) {
		Move playerMove;
		std::cout << "Type in your desired move in the following format: 'start square'->'endsquare' (ex. A1->A2)" << std::endl;
		std::cout << "type 'q' to quit" << std::endl;
		std::cin >> moveString;

		if (moveString == "q") {
			return playerMove;
		}
		
		if (moveString[0] < 'A' || moveString[0] > 'H') {
			std::cout << "Invalid first square" << std::endl;
			continue;
		}
		if (moveString[1] < '1' || moveString[1] > '8') {
			std::cout << "Invalid first square" << std::endl;
			continue;
		}
		if (moveString[4] < 'A' || moveString[4] > 'H') {
			std::cout << "Invalid second square" << std::endl;
			continue;
		}
		if (moveString[5] < '1' || moveString[5] > '8') {
			std::cout << "Invalid second square" << std::endl;
			continue;
		}

		firstSquare = (moveString[0] - 65) + (moveString[1] - 49) * 8;
		secondSquare = (moveString[4] - 65) + (moveString[5] - 49) * 8;


		playerMove.startSQ = (BoardSquare)firstSquare;
		playerMove.endSQ = (BoardSquare)secondSquare;
		if (chessBoard->returnBB().boardSquares[secondSquare] != EMPTY) {
			playerMove.moveType = CAP;
			playerMove.capturedPiece = chessBoard->returnBB().boardSquares[secondSquare];
		}
		else {
			playerMove.moveType = MOVE;
		}
		if (chessBoard->checkPlayerMove(playerMove)) {
			return playerMove;
		}
		else {
			std::cout << "Invalid Move" << std::endl;
			continue;
		}
		return playerMove;
	}
	

}

