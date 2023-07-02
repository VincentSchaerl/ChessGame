#pragma once
#include "Move.h"
#include "Piece.h"

class Board
{
public:
// variables
	Piece* board[9][9];
	bool castlingInformation[6];
	int latestMove[4];
// functions
	Board(const Color& playerColor);
	void clear();
	void setLatestMove(const Move& latestMove);
};