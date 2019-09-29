#ifndef DataIncluded
#define DataIncluded
#include "globals.h"


#include <string>
#include <algorithm>
#include <iostream>

struct BoardBBs {
	//array for board A1 = 0, B1 = 1, ... H8 = 63
	PieceType boardSquares[H8 + 2];	//64 size array that holds what piecetype is on what square(enum has ER type)
									//**see coding notebook for square navigation

	//holds 64bit bitboards for all pieces 
	BBint64 piecesBitBoard[B_KING + 2];	//is +2 becuase it also had a bitboard for EMPTY 

	//utiltiy bitboards
	BBint64 emptySquares;	//bitboard for empty squares
	BBint64 occupiedSquares; //bitboard for occupied squares
	BBint64 colorPieces[2]; //bitboard for each sides total pieces; 0 = white, 1 = black
};


struct Move {
	Move() {
		startSQ = ER;
		endSQ = ER;
		moveType = FAILED;
		capturedPiece = INVALID;
	}
	Move(BoardSquare start, BoardSquare end, MoveType type, PieceType capture) {
		startSQ = start;
		endSQ = end;
		moveType = type;
		capturedPiece = capture;
	}
	Move(BoardSquare start, BoardSquare end, MoveType type) {
		startSQ = start;
		endSQ = end;
		moveType = type;
		capturedPiece = INVALID;
	}
	BoardSquare endSQ;
	BoardSquare startSQ;
	MoveType moveType;
	PieceType capturedPiece;

	void printMove() {
		std::cout << std::to_string(startSQ )+ "->" + std::to_string(endSQ);
	}

	bool operator==(const Move &m1) {
		if (m1.startSQ != startSQ) {
			return false;
		}
		if (m1.endSQ != endSQ) {
			return false;
		}
		if (m1.moveType != moveType) {
			return false;
		}
		if (m1.capturedPiece != capturedPiece) {
			return false;
		}
		return true;
	}

};

struct BoardState {				//boardState historyt to more easily unmake moves
	BoardState(Move past) 
	:move(past.startSQ,past.endSQ, past.moveType, past.capturedPiece)
	{
		
	}
	BBint64 bb;
	bool inCheck;
	int repetitions;
	BoardSquare enPassentSquare;
	Move move;
	int castleRights;
	int numPieces[B_KING + 1];		//hold the number of each of the pieces on the board 
};

#endif
