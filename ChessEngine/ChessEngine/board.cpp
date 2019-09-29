#include "board.h"
#include "globals.h"

#include <math.h>
#include <limits>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>

struct Move;
struct BoardState;
struct BoardBBs;

typedef std::vector<BoardState*> BoardHistory;



Board::Board() {
	setFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	Move m(ER, ER, START);
	updateBoardHist(m);

}

Board::~Board() {

}

int Board::findMaterialScore(BoardState b) {
	int ret = 0;
	ret += kingVal * (b.numPieces[W_KING] - b.numPieces[B_KING]);				//finds material score based on remaining pieces and their value
	ret += queenVal * (b.numPieces[W_QUEEN] - b.numPieces[B_QUEEN]);			//find piece values in globals.h
	ret += rookVal * (b.numPieces[W_ROOK] - b.numPieces[B_ROOK]);		
	ret += bishopVal * (b.numPieces[W_BISHOP] - b.numPieces[B_BISHOP]);
	ret += knightVal * (b.numPieces[W_KNIGHT] - b.numPieces[B_KNIGHT]);
	ret	+= pawnVal * (b.numPieces[W_PAWN] - b.numPieces[B_PAWN]);

	return ret;
}

int Board::findMobilityScore() {
	int ret = 0;
	int whiteMobilityScore = 0;
	int blackMobilityScore = 0;

	for (int i = A1; i <= H8 ; i++) { //cycle through board square
		PieceType piece = bb.boardSquares[i];
		switch (piece) {				//tally up mobility scores based on piece type and what square that piece is on
		case W_PAWN: case W_KNIGHT: case W_BISHOP: case W_ROOK: case W_QUEEN: case W_KING:
			whiteMobilityScore += pieceSquareTable[piece][i];
			break;
		case B_PAWN: case B_KNIGHT: case B_BISHOP: case B_ROOK: case B_QUEEN: case B_KING:
			blackMobilityScore += pieceSquareTable[piece][i];
			break;
		default:
			break;
		}
	}

	ret = (whiteMobilityScore - blackMobilityScore) * mobilityWeight;
	return  ret;
}

int Board::evaluate() {
	
	int materialScore = findMaterialScore(*m_boardHistory.at(m_boardHistory.size() - 1));	//find material score of current boardstate
	int mobilityScore = findMobilityScore();


	int score = (materialScore) + (mobilityScore);		//basic evalute function, change later**
	return score;
}

void Board::makeMove(Move m) {
	bb.boardSquares[m.endSQ] = bb.boardSquares[m.startSQ];
	bb.boardSquares[m.startSQ] = EMPTY;		//makes the move and updates the boardhistory
	
	updateBoardHist(m);
	if (sideToMove == WHITE) {
		sideToMove = BLACK;
	}
	else {
		sideToMove = WHITE;
	}
}

void Board::unmakeMove() {
	Move m = m_boardHistory.back()->move;
	bb.boardSquares[m.startSQ] = bb.boardSquares[m.endSQ];	//sets moved piece back to original position
	if (m.moveType == CAP) {
		bb.boardSquares[m.endSQ] = m.capturedPiece;			//either makes the old position the piece captures or empty 
	}
	else {
		bb.boardSquares[m.endSQ] = EMPTY;
	}
	m_boardHistory.pop_back();

	if (sideToMove == WHITE) {
		sideToMove = BLACK;
	}
	else {
		sideToMove = WHITE;
	}
}

Move Board::negaMaxCaller() {
	int score;
	int max = std::numeric_limits<int>::min();
	std::string bestPlayString = "AI can't find valid move";
	Move bestPlayMove;
	std::vector<Move> engineMoves = getAllMoves();
	
	for (int i = 0; i < engineMoves.size(); i++) {
		std::string currentplayString = std::to_string(engineMoves[i].startSQ) + "->" + std::to_string(engineMoves[i].endSQ);
		if (m_boardHistory.back()->inCheck) {
			if (!checkInCheck(engineMoves[i])) {
				continue;
			}
			else {
				makeMove(engineMoves[i]);
			}
		}
		else {
			makeMove(engineMoves[i]);
		}

		int score = negaMax(defaultSearchDepth, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), sideToMove);
		if (score > max) {
			bestPlayString = currentplayString;
			bestPlayMove = engineMoves[i];
			max = score;
		}
		score = 0;
		unmakeMove();
	}
	std::cout << "The AI has chosen the move: " + bestPlayString << std::endl;
	return bestPlayMove;
}

int Board::negaMax(int depth, int alpha, int beta, int color) {
	if (depth == 0) {
		return -color * evaluate();
	}

	int value;
	std::vector<Move> engineMoves = getAllMoves();
	value = std::numeric_limits<int>::min();
	for (int i = 0; i < engineMoves.size(); i++) {
		
		makeMove(engineMoves[i]);
		value = std::max(value, -negaMax(depth - 1, -beta, -alpha, -color));
		alpha = std::max(alpha, value);
		if (alpha > beta) {
			unmakeMove();
			break;
		}
		unmakeMove();
	}
	return value;
}


void Board::initializeBitBoards() {
	memset(&bb.piecesBitBoard, 0, sizeof(bb.piecesBitBoard));		//sets all of the bitboards to value of 0
	for (BoardSquare sq = A1; sq <= H8; sq++) {
		bb.piecesBitBoard[bb.boardSquares[sq]] |= (C64(1) << sq);		//takes the piece on the current square and sets corresponding bit to 1
	}

	//calculate all the utility bitboards we use
	//white pieces
	bb.colorPieces[0] = bb.piecesBitBoard[W_PAWN] | bb.piecesBitBoard[W_KNIGHT] | bb.piecesBitBoard[W_BISHOP]
		| bb.piecesBitBoard[W_ROOK] | bb.piecesBitBoard[W_QUEEN] | bb.piecesBitBoard[W_KING];
	//black pieces
	bb.colorPieces[1] = bb.piecesBitBoard[B_PAWN] | bb.piecesBitBoard[B_KNIGHT] | bb.piecesBitBoard[B_BISHOP]
		| bb.piecesBitBoard[B_ROOK] | bb.piecesBitBoard[B_QUEEN] | bb.piecesBitBoard[B_KING];

	bb.occupiedSquares = bb.colorPieces[0] | bb.colorPieces[1];
	bb.emptySquares = ~bb.occupiedSquares;
}

BoardBBs Board::returnBB() {
	return bb;
}

int Board::setFEN(std::string aFEN)
{
	unsigned int i, j, state;
	BoardSquare sq;
	char letter;
	int aRank, aFile;
	
	std::vector<std::string> strList;
	std::istringstream iss(aFEN);
	for (std::string s; iss >> s; )
		strList.push_back(s);



	// Empty the board quares
	for (sq = A1; sq <= H8; sq++) bb.boardSquares[sq] = EMPTY;
	// read the board - translate each loop idx into a square
	j = 1; i = 0;
	while ((j <= 64) && (i < strList[0].length()))
	{
		letter = strList[0].at(i);
		i++;
		aFile = 1 + ((j - 1) % 8);
		aRank = 8 - ((j - 1) / 8);
		sq = (BoardSquare)(((aRank - 1) * 8) + (aFile - 1));
		switch (letter)
		{
		case 'p': bb.boardSquares[sq] = B_PAWN; break;
		case 'r': bb.boardSquares[sq] = B_ROOK; break;
		case 'n': bb.boardSquares[sq] = B_KNIGHT; break;
		case 'b': bb.boardSquares[sq] = B_BISHOP; break;
		case 'q': bb.boardSquares[sq] = B_QUEEN; break;
		case 'k': bb.boardSquares[sq] = B_KING; break;
		case 'P': bb.boardSquares[sq] = W_PAWN; break;
		case 'R': bb.boardSquares[sq] = W_ROOK; break;
		case 'N': bb.boardSquares[sq] = W_KNIGHT; break;
		case 'B': bb.boardSquares[sq] = W_BISHOP; break;
		case 'Q': bb.boardSquares[sq] = W_QUEEN; break;
		case 'K': bb.boardSquares[sq] = W_KING; break;
		case '/': j--; break;
		case '1': break;
		case '2': j++; break;
		case '3': j += 2; break;
		case '4': j += 3; break;
		case '5': j += 4; break;
		case '6': j += 5; break;
		case '7': j += 6; break;
		case '8': j += 7; break;
		default: return -1;
		}
		j++;
	}
	sideToMove = WHITE;
	if (strList.size() >= 2)
	{
		if (strList[1] == "w") sideToMove = WHITE; else
			if (strList[1] == "b") sideToMove = BLACK; else return -1;
	}

	//insert setting up castling rights here
	//
	//
	//
	//
	//
	//

	// read en passent and save it into "sq" Default := None (ER)
	sq = ER;
	if ((strList.size() >= 4) && (strList[3].length() >= 2))
	{
		if ((strList[3].at(0) >= 'a') && (strList[3].at(0) <= 'h') &&
			((strList[3].at(1) == '3') || (strList[3].at(1) == '6')))
		{
			aFile = strList[3].at(0) - 96; // ASCII 'a' = 97
			aRank = strList[3].at(1) - 48; // ASCII '1' = 49
			sq = (BoardSquare)((aRank - 1) * 8 + aFile - 1);
		}
		else return -1;
	}

	// we start at turn 1 per default
	currentPly = 1;
	if (strList.size() >= 6)
	{
		currentPly = 2 * (stoi(strList[5]) - 1) + 1;
		if (currentPly < 0) currentPly = 0; // avoid possible underflow
		if (sideToMove == BLACK) currentPly++;
	}

	initializeBitBoards();
	return 0;

}

void Board::updateBoardHist(Move m) {
	BoardState* temp = new BoardState(m);
	temp->bb = this->bb.occupiedSquares;
	temp->inCheck = false;
	temp->repetitions = 0;

	
	std::vector<Move> checkList = getAllMoves();			//make the desired move and check if all possible moves for the opponent dont involve capturing the king
	for (int i = 0; i < checkList.size(); i++) {
		if (checkList[i].moveType == CAP && checkList[i].capturedPiece == B_KING) {
			temp->inCheck = true;
			break;
		}
	}

	for (int i = 0; i < 64; i++) {
		temp->numPieces[this->bb.boardSquares[i]]++;
	}
	m_boardHistory.push_back(temp);
}

//move generation functions
std::vector<Move> Board::getRookMoves(BoardSquare sq) {
	BoardSquare scan = sq;			//we are going to traverse the array with a seperate BoardSquare
	std::vector<Move> ret;

	for (int i = 0; i < 4; i++) {
		scan += rookDirections[i];
		while (returnBB().boardSquares[scan] == EMPTY && scan <= H8 && scan >= A1) {	//go until it finds a non Empty Square
			if (rookDirections[i] == EAST) {			//should catch wrap arounds off right side 
				if (scan % 8 == 0) {
					break;
				}
			}
			if (rookDirections[i] == WEST) {			//checks wrap arounds off left side 
				if (scan % 8 == 7) {
					break;
				}
			}
			Move newMove(sq, scan, MOVE);
			ret.push_back(newMove);
			scan += rookDirections[i];
		}
		if (sideToMove == WHITE) {
			if (returnBB().boardSquares[scan] > 5 && returnBB().boardSquares[scan] < 12 && scan <= H8 && scan >= A1) {
				ret.push_back(Move(sq, scan, CAP, returnBB().boardSquares[scan]));		//gives option to capture first piece it encounters 
			}
		}

		if (sideToMove == BLACK) {
			if (returnBB().boardSquares[scan] <= 5 && scan <= H8 && scan >= A1) {
				ret.push_back(Move(sq, scan, CAP, returnBB().boardSquares[scan]));		//gives option to capture first piece it encounters 
			}
		}
		scan = sq;
	}
	return ret;
}

std::vector<Move> Board::getBishopMoves(BoardSquare sq) {
	BoardSquare scan = sq;			//we are going to traverse the array with a seperate BoardSquare
	std::vector<Move> ret;

	for (int i = 0; i < 4; i++) {
		scan += bishopDirections[i];
		while (returnBB().boardSquares[scan] == EMPTY && scan <= H8 && scan >= A1) {	//go until it finds a non Empty Square
			if (bishopDirections[i] == NORTHEAST || bishopDirections[i] == SOUTHEAST) {			//should catch wrap arounds off right side 
				if (scan % 8 == 0) {
					scan = sq;
					break;
				}
			}
			if (bishopDirections[i] == NORTHWEST || bishopDirections[i] == SOUTHWEST) {			//checks wrap arounds off left side 
				if (scan % 8 == 7) {
					scan = sq;
					break;
				}
			}
			Move newMove(sq, scan, MOVE);
			ret.push_back(newMove);
			scan += bishopDirections[i];
		}
		if (sideToMove == WHITE) {
			if (bishopDirections[i] == NORTHEAST || bishopDirections[i] == SOUTHEAST) {			//should catch wrap arounds off right side 
				if (scan % 8 == 0) {
					scan = sq;
					continue;
				}
			}
			if (bishopDirections[i] == NORTHWEST || bishopDirections[i] == SOUTHWEST) {			//checks wrap arounds off left side 
				if (scan % 8 == 7) {
					scan = sq;
					continue;
				}
			}
			if (returnBB().boardSquares[scan] > 5 && returnBB().boardSquares[scan] < 12 && scan <= H8 && scan >= A1) {
				ret.push_back(Move(sq, scan, CAP, returnBB().boardSquares[scan]));		//gives option to capture first piece it encounters 
			}
		}

		if (sideToMove == BLACK) {
			if (bishopDirections[i] == NORTHEAST || bishopDirections[i] == SOUTHEAST) {			//should catch wrap arounds off right side 
				if (scan % 8 == 0) {
					scan = sq;
					continue;
				}
			}
			if (bishopDirections[i] == NORTHWEST || bishopDirections[i] == SOUTHWEST) {			//checks wrap arounds off left side 
				if (scan % 8 == 7) {
					scan = sq;
					continue;
				}
			}
			if (returnBB().boardSquares[scan] <= 5 && scan <= H8 && scan >= A1) {
				ret.push_back(Move(sq, scan, CAP, returnBB().boardSquares[scan]));		//gives option to capture first piece it encounters 
			}
		}
		scan = sq;
	}
	return ret;
}

std::vector<Move> Board::getQueenMoves(BoardSquare sq) {
	BoardSquare scan = sq;			//we are going to traverse the array with a seperate BoardSquare
	std::vector<Move> ret;

	for (int i = 0; i < 8; i++) {
		scan += queenDirections[i];
		while (returnBB().boardSquares[scan] == EMPTY && scan <= H8 && scan >= A1) {	//go until it finds a non Empty Square
			if (queenDirections[i] == NORTHEAST || queenDirections[i] == SOUTHEAST || queenDirections[i] == EAST) {			//should catch wrap arounds off right side 
				if (scan % 8 == 0) {
					scan = sq;
					break;
				}
			}
			if (queenDirections[i] == NORTHWEST || queenDirections[i] == SOUTHWEST || queenDirections[i] == WEST) {			//checks wrap arounds off left side 
				if (scan % 8 == 7) {
					scan = sq;
					break;
				}
			}
			Move newMove(sq, scan, MOVE);
			ret.push_back(newMove);
			scan += queenDirections[i];
		}
		if (sideToMove == WHITE) {
			if (queenDirections[i] == NORTHEAST || queenDirections[i] == SOUTHEAST || queenDirections[i] == EAST) {			//should catch wrap arounds off right side 
				if (scan % 8 == 0) {
					scan = sq;
					continue;
				}
			}
			if (queenDirections[i] == NORTHWEST || queenDirections[i] == SOUTHWEST || queenDirections[i] == WEST) {			//checks wrap arounds off left side 
				if (scan % 8 == 7) {
					scan = sq;
					continue;
				}
			}
			if (returnBB().boardSquares[scan] > 5 && returnBB().boardSquares[scan] < 12 && scan <= H8 && scan >= A1) {
				ret.push_back(Move(sq, scan, CAP, returnBB().boardSquares[scan]));		//gives option to capture first piece it encounters 
			}
		}

		if (sideToMove == BLACK) {
			if (queenDirections[i] == NORTHEAST || queenDirections[i] == SOUTHEAST || queenDirections[i] == EAST) {			//should catch wrap arounds off right side 
				if (scan % 8 == 0) {
					scan = sq;
					continue;
				}
			}
			if (queenDirections[i] == NORTHWEST || queenDirections[i] == SOUTHWEST || queenDirections[i] == WEST) {			//checks wrap arounds off left side 
				if (scan % 8 == 7) {
					scan = sq;
					continue;
				}
			}
			if (returnBB().boardSquares[scan] <= 5 && scan <= H8 && scan >= A1) {
				ret.push_back(Move(sq, scan, CAP, returnBB().boardSquares[scan]));		//gives option to capture first piece it encounters 
			}
		}
		scan = sq;
	}
	return ret;
}

std::vector<Move> Board::getKingMoves(BoardSquare sq) {
	BoardSquare scan = sq;			//we are going to traverse the array with a seperate BoardSquare
	std::vector<Move> ret;

	for (int i = 0; i < 8; i++) {
		scan += queenDirections[i];
		if (returnBB().boardSquares[scan] == EMPTY && scan <= H8 && scan >= A1) {	//go until it finds a non Empty Square
			if (queenDirections[i] == NORTHEAST || queenDirections[i] == SOUTHEAST || queenDirections[i] == EAST) {			//should catch wrap arounds off right side 
				if (scan % 8 == 0) {
					scan = sq;
					continue;
				}
			}
			if (queenDirections[i] == NORTHWEST || queenDirections[i] == SOUTHWEST || queenDirections[i] == WEST) {			//checks wrap arounds off left side 
				if (scan % 8 == 7) {
					scan = sq;
					continue;
				}
			}
			Move newMove(sq, scan, MOVE);
			ret.push_back(newMove);
			scan = sq;
		}
		if (sideToMove == WHITE) {
			if (queenDirections[i] == NORTHEAST || queenDirections[i] == SOUTHEAST || queenDirections[i] == EAST) {			//should catch wrap arounds off right side 
				if (scan % 8 == 0) {
					scan = sq;
					continue;
				}
			}
			if (queenDirections[i] == NORTHWEST || queenDirections[i] == SOUTHWEST || queenDirections[i] == WEST) {			//checks wrap arounds off left side 
				if (scan % 8 == 7) {
					scan = sq;
					continue;
				}
			}
			if (returnBB().boardSquares[scan] > 5 && returnBB().boardSquares[scan] < 12 && scan <= H8 && scan >= A1) {
				ret.push_back(Move(sq, scan, CAP, returnBB().boardSquares[scan]));		//gives option to capture first piece it encounters 
			}
		}

		if (sideToMove == BLACK) {
			if (queenDirections[i] == NORTHEAST || queenDirections[i] == SOUTHEAST || queenDirections[i] == EAST) {			//should catch wrap arounds off right side 
				if (scan % 8 == 0) {
					scan = sq;
					continue;
				}
			}
			if (queenDirections[i] == NORTHWEST || queenDirections[i] == SOUTHWEST || queenDirections[i] == WEST) {			//checks wrap arounds off left side 
				if (scan % 8 == 7) {
					scan = sq;
					continue;
				}
			}
			if (returnBB().boardSquares[scan] <= 5 && scan <= H8 && scan >= A1) {
				ret.push_back(Move(sq, scan, CAP, returnBB().boardSquares[scan]));		//gives option to capture first piece it encounters 
			}
		}
		scan = sq;
	}
	return ret;
}

std::vector<Move> Board::getKnightMoves(BoardSquare sq) {
	BoardSquare scan = sq;
	std::vector<Move> ret;
	for (int i = 0; i < 8; i++) {
		scan += knightDirections[i];
		if (scan > H8 || scan < A1) {
			scan = sq;
			continue;
		}
		if (abs((scan % 8) - (sq % 8)) > 3) {
			scan = sq;
			continue;
		}
		if (sideToMove == WHITE) {
			if (returnBB().boardSquares[scan] == EMPTY) {
				ret.push_back(Move(sq, scan, MOVE));
			}
			else if (returnBB().boardSquares[scan] > 5 && returnBB().boardSquares[scan] < 12 && scan <= H8 && scan >= A1) {
				ret.push_back(Move(sq, scan, CAP, returnBB().boardSquares[scan]));		//gives option to capture first piece it encounters 
			}
		}

		if (sideToMove == BLACK) {
			if (returnBB().boardSquares[scan] == EMPTY) {
				ret.push_back(Move(sq, scan, MOVE));
			}
			else if (returnBB().boardSquares[scan] <= 5 && scan <= H8 && scan >= A1) {
				ret.push_back(Move(sq, scan, CAP, returnBB().boardSquares[scan]));		//gives option to capture first piece it encounters 
			}
		}
		scan = sq;
	}

	return ret;
}

std::vector<Move> Board::getPawnMoves(BoardSquare sq) {
	int i;
	int max;
	if (sideToMove == WHITE) {
		i = 0;
		max = 3;
	}
	else {
		i = 3;
		max = 6;
	}
	BoardSquare scan = sq;
	std::vector<Move> ret;
	for (; i < max; i++) {
		scan += queenDirections[i];
		if (scan > H8 || scan < A1) {
			scan = sq;
			continue;
		}
		if (abs((scan % 8) - (sq % 8)) > 3) {
			scan = sq;
			continue;
		}
		if (sideToMove == WHITE) {
			if (queenDirections[i] == NORTHWEST || queenDirections[i] == NORTHEAST) {
				if (returnBB().boardSquares[scan] > 5 && returnBB().boardSquares[scan] < 12 && scan <= H8 && scan >= A1) {
					ret.push_back(Move(sq, scan, CAP, returnBB().boardSquares[scan]));		//gives option to capture first piece it encounters 
				}
				else {
					scan = sq;
					continue;
				}
			}
			else if (returnBB().boardSquares[scan] == EMPTY) {
				ret.push_back(Move(sq, scan, MOVE));
				if (sq >= 8 && sq <= 15 && returnBB().boardSquares[scan + 8] == EMPTY) {
					scan += 8;
					ret.push_back(Move(sq, scan, MOVE));
				}
			}

		}

		else if (sideToMove == BLACK) {
			if (queenDirections[i] == SOUTHWEST || queenDirections[i] == SOUTHEAST) {
				if (returnBB().boardSquares[scan] > 5 && returnBB().boardSquares[scan] < 12 && scan <= H8 && scan >= A1) {
					ret.push_back(Move(sq, scan, CAP, returnBB().boardSquares[scan]));		//gives option to capture first piece it encounters 
				}
				else {
					scan = sq;
					continue;
				}
			}
			else if (returnBB().boardSquares[scan] == EMPTY) {
				ret.push_back(Move(sq, scan, MOVE));
				if (sq >= 48 && sq <= 55 && returnBB().boardSquares[scan - 8] == EMPTY) {
					scan -= 8;
					ret.push_back(Move(sq, scan, MOVE));
				}
			}
		}
		scan = sq;
	}

	return ret;
}

std::vector<Move> Board::getAllMoves() {
	std::vector<Move> ret;
	if (sideToMove == WHITE) {
		for (BoardSquare sq = A1; sq < 64; sq++) {
			PieceType piece = returnBB().boardSquares[sq];
			if (piece <= 5) {
				std::vector<Move> temp;
				switch (piece) {
				case W_PAWN:
					temp = getPawnMoves(sq);
					ret.insert(ret.end(), temp.begin(), temp.end());
					break;
				case W_ROOK:
					temp = getRookMoves(sq);
					ret.insert(ret.end(), temp.begin(), temp.end());
					break;
				case W_KNIGHT:
					temp = getKnightMoves(sq);
					ret.insert(ret.end(), temp.begin(), temp.end());
					break;
				case W_BISHOP:
					temp = getBishopMoves(sq);
					ret.insert(ret.end(), temp.begin(), temp.end());
					break;
				case W_QUEEN:
					temp = getQueenMoves(sq);
					ret.insert(ret.end(), temp.begin(), temp.end());
					break;
				case W_KING:
					temp = getKingMoves(sq);
					ret.insert(ret.end(), temp.begin(), temp.end());
					break;
				}
			}
		}
	}
	else {
		for (BoardSquare sq = A1; sq < 64; sq++) {
			PieceType piece = returnBB().boardSquares[sq];
			if (piece > 5 && piece < 12) {
				std::vector<Move> temp;
				switch (piece) {
				case B_PAWN:
					temp = getPawnMoves(sq);
					ret.insert(ret.end(), temp.begin(), temp.end());
					break;
				case B_ROOK:
					temp = getRookMoves(sq);
					ret.insert(ret.end(), temp.begin(), temp.end());
					break;
				case B_KNIGHT:
					temp = getKnightMoves(sq);
					ret.insert(ret.end(), temp.begin(), temp.end());
					break;
				case B_BISHOP:
					temp = getBishopMoves(sq);
					ret.insert(ret.end(), temp.begin(), temp.end());
					break;
				case B_QUEEN:
					temp = getQueenMoves(sq);
					ret.insert(ret.end(), temp.begin(), temp.end());
					break;
				case B_KING:
					temp = getKingMoves(sq);
					ret.insert(ret.end(), temp.begin(), temp.end());
					break;
				}
			}
		}
	}
	return ret;
}

void Board::printBoard() {
	int counter = 7;
	std::cout << "8 ";
	for (int i = 56; i >= 0; i++) {
		std::cout << "|";
		PieceType p = bb.boardSquares[i];
		switch (p)
		{
		case W_PAWN:
			std::cout << "P";
			break;
		case W_KNIGHT:
			std::cout << "N";
			break;
		case W_BISHOP:
			std::cout << "B";
			break;
		case W_ROOK:
			std::cout << "R";
			break;
		case W_QUEEN:
			std::cout << "Q";
			break;
		case W_KING:
			std::cout << "K";
			break;
		case B_PAWN:
			std::cout << "p";
			break;
		case B_KNIGHT:
			std::cout << "n";
			break;
		case B_BISHOP:
			std::cout << "b";
			break;
		case B_ROOK:
			std::cout << "r";
			break;
		case B_QUEEN:
			std::cout << "q";
			break;
		case B_KING:
			std::cout << "k";
			break;
		case EMPTY:
			std::cout << " ";
			break;
		case INVALID:
			std::cout << "I";
			break;
		default:
			std::cout << "X";
			break;
		}
		if (i % 8 == 7) {
			std::cout << "|" << std::endl;
			i -= 16;
			if (i >= -8) {
				std::cout << counter << " ";
				counter--;
			}
		}
	}
	std::cout << "   A B C D E F G H";
	std::cout << std::endl << std::endl;
}

bool Board::checkPlayerMove(Move m) {
	makeMove(m);
	std::vector<Move> checkList = getAllMoves();			//make the desired move and check if all possible moves for the opponent dont involve capturing the king
	for (int i = 0; i < checkList.size(); i++) {
		if (checkList[i].moveType == CAP && checkList[i].capturedPiece == W_KING) {
			unmakeMove();
			return false;
		}
	}
	unmakeMove();

	checkList = getAllMoves();
	for (int i = 0; i < checkList.size(); i++) {
		if (m == checkList[i]) {
			return true;
		}
	}
	return false;
}

bool Board::checkInCheck(Move m) {
	makeMove(m);
	std::vector<Move> checkList = getAllMoves();			//make the desired move and check if all possible moves for the opponent dont involve capturing the king
	for (int i = 0; i < checkList.size(); i++) {
		if (checkList[i].moveType == CAP && checkList[i].capturedPiece == B_KING) {
			unmakeMove();
			return false;
		}
	}
	unmakeMove();
	return true;
}