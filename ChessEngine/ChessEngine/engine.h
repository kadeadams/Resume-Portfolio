#pragma once
#include "board.h"
#include "boardData.h"
#include<string>

class ChessEngine {
public:
	ChessEngine();
	~ChessEngine();
	void play();

private:
	Move getPlay();
	Board* chessBoard;

};
