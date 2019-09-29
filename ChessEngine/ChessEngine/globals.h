#ifndef GlobalsIncluded
#define GlobalsIncluded
#include <math.h>
#define C64(constantBBint64) constantBBint64##ULL


typedef unsigned long long BBint64;

//bitboard constants for little endian rank-file(LERF) mapping 
const unsigned int long long fullBB = 0xffffffffffffffff;const BBint64 aFile = 0x0101010101010101;
const BBint64 hFile = 0x8080808080808080;
const BBint64 firstRank = 0x00000000000000FF;
const BBint64 eigthRank = 0xFF00000000000000;
const BBint64 a1_h8Diagonal = 0x8040201008040201;
const BBint64 h1_a8Diagonal = 0x0102040810204080;
const BBint64 lightSquares = 0x55AA55AA55AA55AA;
const BBint64 darkSquares = 0xAA55AA55AA55AA55;

//directional constants based on mapping(a1 = 0, b1 = 1, .... h8 = 63)
const int NORTH = 8;
const int SOUTH = -8;
const int WEST = -1;
const int EAST = 1;

const int NORTHWEST = 7;
const int NORTHEAST = 9;
const int SOUTHWEST = -9;
const int SOUTHEAST = -7;

const int queenDirections [8] = { NORTH, NORTHWEST, NORTHEAST, SOUTH,  SOUTHWEST, SOUTHEAST, EAST, WEST };
const int rookDirections[4] = { NORTH, SOUTH, EAST, WEST };
const int bishopDirections[4] = { NORTHEAST, NORTHWEST, SOUTHWEST, SOUTHEAST };
const int knightDirections[8] = { -17, -15, -6, 10, 17, 15, 6, -10 };


//default search depth for negaMax function
const int defaultSearchDepth = 3;


//piece value constants
const int pawnVal = 100;		//piece values to give a value to each general piece
const int knightVal = 320;		//can change values based on the general behavior you want the engine to follow 
const int bishopVal = 330;
const int rookVal = 500;
const int queenVal = 900;
const int kingVal = 20000;



//enum for our piecetype for readabiltiy
enum PieceType {
	W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
	B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING, EMPTY, INVALID
};
inline
PieceType operator++ (PieceType& piece) {			//this allows us to increment us through our enum 
	return PieceType(piece + 1);
}
inline 
PieceType operator++ (PieceType& piece, int)
{
	PieceType old = piece; 
	piece = (PieceType)(piece + 1); 
	return old;
};



//enum for each board square; unsure if I am going to use this but ould be useful
enum BoardSquare {
	A1, B1, C1, D1, E1, F1, G1, H1,
	A2, B2, C2, D2, E2, F2, G2, H2,
	A3, B3, C3, D3, E3, F3, G3, H3,
	A4, B4, C4, D4, E4, F4, G4, H4,
	A5, B5, C5, D5, E5, F5, G5, H5,
	A6, B6, C6, D6, E6, F6, G6, H6,
	A7, B7, C7, D7, E7, F7, G7, H7,
	A8, B8, C8, D8, E8, F8, G8, H8, ER
};
inline
BoardSquare operator++ (BoardSquare& square) {			//this allows us to increment us through our enum 
	return BoardSquare(square + 1);
}
inline
BoardSquare operator++ (BoardSquare& board, int)
{
	BoardSquare old = board;
	board = (BoardSquare)(board + 1);
	return old;
};
inline
BoardSquare operator+= (BoardSquare& square, int i) {
	square = (BoardSquare)(square + i);
	return square;
}
inline
BoardSquare operator-= (BoardSquare& square, int i) {
	square = (BoardSquare)(square - i);
	return square;
}
enum MoveType {
	MOVE, CAP, ENPASS, CASTLE, FAILED, START
};

//weight we give the engine for prioritizing mobility
const int mobilityWeight = 1;

//square value constants (piece specific)
const int pieceSquareTable[B_KING + 1][H8 + 1] = {
//white pawn
{0,  0,  0,  0,  0,  0,  0,  0,				//kinda of confusing but top left[0] is A1 then B1 is [1](its essentially inverted because its LERF mapping
 5, 10, 10,-20,-20, 10, 10,  5,
 5, -5,-10,  0,  0,-10, -5,  5,
 0,  0,  0, 20, 20,  0,  0,  0,
 5,  5, 10, 25, 25, 10,  5,  5,
10, 10, 20, 30, 30, 20, 10, 10,
50, 50, 50, 50, 50, 50, 50, 50,
 0,  0,  0,  0,  0,  0,  0,  0},
 //white knight
{-50,-40,-30,-30,-30,-30,-40,-50,
-40,-20,  0,  5,  5,  0,-20,-40,
-30,  5, 10, 15, 15, 10,  5,-30,
-30,  0, 15, 20, 20, 15,  0,-30,
-30,  5, 15, 20, 20, 15,  5,-30,
-30,  0, 10, 15, 15, 10,  0,-30,
-40,-20,  0,  0,  0,  0,-20,-40,
-50,-40,-30,-30,-30,-30,-40,-50},
//white bishop
{-20,-10,-10,-10,-10,-10,-10,-20,
-10,  5,  0,  0,  0,  0,  5,-10,
-10, 10, 10, 10, 10, 10, 10,-10,
-10,  0, 10, 10, 10, 10,  0,-10,
-10,  5,  5, 10, 10,  5,  5,-10,
-10,  0,  5, 10, 10,  5,  0,-10,
-10,  0,  0,  0,  0,  0,  0,-10,
-20,-10,-10,-10,-10,-10,-10,-20},
//white rook
{ 0,  0,  0,  5,  5,  0,  0,  0,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
  5, 10, 10, 10, 10, 10, 10,  5,
  0,  0,  0,  0,  0,  0,  0,  0,},
  //white queen
{-20,-10,-10, -5, -5,-10,-10,-20,
 -10,  0,  0,  0,  0,  5,  0,-10,
 -10,  0,  5,  5,  5,  5,  5,-10,
  -5,  0,  5,  5,  5,  5,  0, 0,
  -5,  0,  5,  5,  5,  5,  0, -5,
 -10,  0,  5,  5,  5,  5,  0,-10,
 -10,  0,  0,  0,  0,  0,  0,-10,
 -20,-10,-10, -5, -5,-10,-10,-20},
 //white king			
{20, 30, 10,  0,  0, 10, 30, 20,				//this is for king middle game, should implement end game later
 20, 20,  0,  0,  0,  0, 20, 20,
-10,-20,-20,-20,-20,-20,-20,-10,
-20,-30,-30,-40,-40,-30,-30,-20,
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30},

//black pawn
{0,  0,  0,  0,  0,  0,  0,  0,				//kinda of confusing but top left[0] is A1 then B1 is [1]
50, 50, 50, 50, 50, 50, 50, 50,
10, 10, 20, 30, 30, 20, 10, 10,
 5,  5, 10, 25, 25, 10,  5,  5,
 0,  0,  0, 20, 20,  0,  0,  0,
 5, -5,-10,  0,  0,-10, -5,  5,
 5, 10, 10,-20,-20, 10, 10,  5,
 0,  0,  0,  0,  0,  0,  0,  0},

 //black knight
{-50,-40,-30,-30,-30,-30,-40,-50,
-40,-20,  0,  0,  0,  0,-20,-40,
-30,  0, 10, 15, 15, 10,  0,-30,
-30,  5, 15, 20, 20, 15,  5,-30,
-30,  0, 15, 20, 20, 15,  0,-30,
-30,  5, 10, 15, 15, 10,  5,-30,
-40,-20,  0,  5,  5,  0,-20,-40,
-50,-40,-30,-30,-30,-30,-40,-50},
//black bishop
{-20,-10,-10,-10,-10,-10,-10,-20,
-10,  0,  0,  0,  0,  0,  0,-10,
-10,  0,  5, 10, 10,  5,  0,-10,
-10,  5,  5, 10, 10,  5,  5,-10,
-10,  0, 10, 10, 10, 10,  0,-10,
-10, 10, 10, 10, 10, 10, 10,-10,
-10,  5,  0,  0,  0,  0,  5,-10,
-20,-10,-10,-10,-10,-10,-10,-20},
//black rook
{0,  0,  0,  0,  0,  0,  0,  0,
  5, 10, 10, 10, 10, 10, 10,  5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
  0,  0,  0,  5,  5,  0,  0,  0},
  //black queen
{-20,-10,-10, -5, -5,-10,-10,-20,
-10,  0,  0,  0,  0,  0,  0,-10,
-10,  0,  5,  5,  5,  5,  0,-10,
 -5,  0,  5,  5,  5,  5,  0, -5,
  0,  0,  5,  5,  5,  5,  0, -5,
-10,  5,  5,  5,  5,  5,  0,-10,
-10,  0,  5,  0,  0,  0,  0,-10,
-20,-10,-10, -5, -5,-10,-10,-20},
//black king			
{-30,-40,-40,-50,-50,-40,-40,-30,				//this is for king middle game, should implement end game later
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-20,-30,-30,-40,-40,-30,-30,-20,
-10,-20,-20,-20,-20,-20,-20,-10,
 20, 20,  0,  0,  0,  0, 20, 20,
 20, 30, 10,  0,  0, 10, 30, 20},

 //white pawn

};


enum Color {
	WHITE = 1, BLACK = -1, NONE = 0
};

//bitboard global methods
inline 
bool isBBEmpty(BBint64 a) {
	if (a == 0) {
		return true;
	}
	else {
		return false;
	}
}

inline
bool isBBDisjoint(BBint64 a, BBint64 b) {
	if ((a & b) == 0) {
		return true;
	}
	else {
		return false;
	}
}


#endif
