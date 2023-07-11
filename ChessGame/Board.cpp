#include "Board.h"
#include "SupportFunctions.h"

// castlingInformation[0] = king E8 never moved, castlingInformation[1] = rook A8 never moved, castlingInformation[2] = rook H8 never moved
// castlingInformation[3] = king E1 never moved, castlingInformation[4] = rook A1 never moved, castlingInformation[5] = rook H1 never moved

// latestMove[0] = ax of latest move, latestMove[1] = ay of latest move, latestMove[2] = bx of latest move, latestMove[3] = by of latest move

// no explicit copy constructor, because the default copy constructor serves the purpose just fine:
// the board array gets shallow copied, so that the piece objects do not get duplicated (better perfomance),
// but the castlingInformation and latestMove arrays get deep copied, so that there is no corruption of the original values
Board::Board(const Color& playerColor)
{
	Color computerColor = oppositeColor(playerColor);
	board[1][8] = new Rook(computerColor);
	board[2][8] = new Knight(computerColor);
	board[3][8] = new Bishop(computerColor);
	board[4][8] = new Queen(computerColor);
	board[5][8] = new King(computerColor);
	board[6][8] = new Bishop(computerColor);
	board[7][8] = new Knight(computerColor);
	board[8][8] = new Rook(computerColor);
	for (int i = 1; i <= 8; ++i)
	{
		board[i][7] = new Pawn(computerColor, false);
	}
	for (int i = 1; i <= 8; ++i)
	{
		for (int j = 3; j <= 6; ++j)
		{
			board[i][j] = nullptr;
		}
	}
	for (int i = 1; i <= 8; ++i)
	{
		board[i][2] = new Pawn(playerColor, true);
	}
	board[1][1] = new Rook(playerColor);
	board[2][1] = new Knight(playerColor);
	board[3][1] = new Bishop(playerColor);
	board[4][1] = new Queen(playerColor);
	board[5][1] = new King(playerColor);
	board[6][1] = new Bishop(playerColor);
	board[7][1] = new Knight(playerColor);
	board[8][1] = new Rook(playerColor);
	for (int i = 0; i <= 5; ++i)
	{
		castlingInformation[i] = true;
	}
	for (int i = 0; i <= 3; ++i)
	{
		latestMove[i] = 0;
	}
}

void Board::clear()
{
	for (int i = 1; i <= 8; ++i)
	{
		for (int j = 1; j <= 8; ++j)
		{
			delete board[i][j];
			board[i][j] = nullptr;
		}
	}
}

void Board::setLatestMove(const Move& move)
{
	latestMove[0] = move.ax;
	latestMove[1] = move.ay;
	latestMove[2] = move.bx;
	latestMove[3] = move.by;
}