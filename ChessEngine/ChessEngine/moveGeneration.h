#pragma once


#include "globals.h"



//overarching generation for attacksets
//returns a 64bit integer representing a moveset
BBint64 generateAttackSet(PieceType p, BBint64 piecePosition) {
	switch (p) {
	default:
		break;
	}
	return 0;
}



BBint64 rookMoves(BBint64 pos) {
	BBint64 vertAttack = hFile;
	BBint64 horizAttack = eigthRank;
	while (!vertAttack & pos) {	//because we have a sliding piece shift the afile and 8th rank until they match with the piece 
		vertAttack << 1;
	}
	while (!horizAttack & pos) {
		vertAttack << 8;
	}
	BBint64 attackSet = vertAttack ^ horizAttack;
	BBint64 returnSet = ~(attackSet & )
}
