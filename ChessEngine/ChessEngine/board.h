#ifndef BoardIncluded
#define BoardIncluded

#include <vector>
#include <string>


#include "globals.h"
#include "boardData.h"


typedef std::vector<BoardState*> BoardHistory;

class Board {
public:
	//public functions
	Board();
	~Board();	
	Move negaMaxCaller();			//calls the negamax algorithm with a similar loop
	int evaluate();					//evalute the score of the current board state based on whos turn it is;
	Color sideToMove;				//enum type of whos turn it is to move, globals.h has respective values
	int currentPly;
	BoardBBs returnBB();
	int setFEN(std::string aFEN);
	std::vector<Move> getAllMoves();

	void makeMove(Move m);
	void unmakeMove();
	bool checkPlayerMove(Move m);
	void printBoard();
	


private:
	//private variables
	BoardBBs bb;					//bitboard objects that hold main bitboards, board-centric representation, and utility bitboards		
	BoardHistory m_boardHistory;			//defines earlier as a vector of previous boardstates
	int numberWhitePieces;	
	int numberBlackPieces;

	//private member functions
	std::vector<Move> getRookMoves(BoardSquare sq);
	std::vector<Move> getPawnMoves(BoardSquare sq);
	std::vector<Move> getKnightMoves(BoardSquare sq);
	std::vector<Move> getKingMoves(BoardSquare sq);
	std::vector<Move> getQueenMoves(BoardSquare sq);
	std::vector<Move> getBishopMoves(BoardSquare sq);
	bool checkInCheck(Move m);

	



	//private functions
	void initializeBitBoards();
	int findMaterialScore(BoardState b);
	int findMobilityScore();
	//int alphaBetaMax(int alpha, int beta, int depthleft);
	//int alphaBetaMin(int alpha, int beta, int depthleft);
	int negaMax(int depth, int alpha, int beta, int color);
	void updateBoardHist(Move m);


};

#endif
