
#include "globals.h"
#include "board.h"
#include "engine.h"

#define C64(constantBBint64) constantBBint64##ULL

int main() {
	ChessEngine test;
	test.play();
}