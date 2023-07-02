#include "Piece.h"
#include <typeinfo>
#include "Functions.h"

using std::vector;

Piece::Piece(const Color& color) : color(color) {}

Bishop::Bishop(const Color& color) : Piece(color) {}

King::King(const Color& color) : Piece(color) {}

Knight::Knight(const Color& color) : Piece(color) {}

Pawn::Pawn(const Color& color, bool isPlayer) : Piece(color), isPlayer(isPlayer) {}

Queen::Queen(const Color& color) : Piece(color) {}

Rook::Rook(const Color& color) : Piece(color) {}

char Bishop::getChar()
{
	if (color == black)
	{
		return 'b';
	}
	else
	{
		return 'B';
	}
}

char King::getChar()
{
	if (color == black)
	{
		return 'k';
	}
	else
	{
		return 'K';
	}
}

char Knight::getChar()
{
	if (color == black)
	{
		return 'n';
	}
	else
	{
		return 'N';
	}
}

char Pawn::getChar()
{
	if (color == black)
	{
		return 'p';
	}
	else
	{
		return 'P';
	}
}

char Queen::getChar()
{
	if (color == black)
	{
		return 'q';
	}
	else
	{
		return 'Q';
	}
}

char Rook::getChar()
{
	if (color == black)
	{
		return 'r';
	}
	else
	{
		return 'R';
	}
}

// adds all "physically" possible moves to "moves", including moves that are not legal in terms of check
void Bishop::checkMoves(const Board& board, bool checkCastling, vector<Move>& moves, const int* const position)
{
	const int x = position[0];
	const int y = position[1];
	for (int dx = -1; dx <= 1; dx += 2)
	{
		for (int dy = -1; dy <= 1; dy += 2)
		{
			int i = 1;
			while (x + i * dx >= 1 && x + i * dx <= 8 && y + i * dy >= 1 && y + i * dy <= 8)
			{
				// next square is empty
				if (board.board[x + i * dx][y + i * dy] == nullptr)
				{
					moves.push_back(Move(x, y, x + i * dx, y + i * dy));
					++i;
				}
				// next square is occupied by enemy piece
				else if (color != board.board[x + i * dx][y + i * dy]->color)
				{
					moves.push_back(Move(x, y, x + i * dx, y + i * dy));
					break;
				}
				// next square is occupied by piece of own color
				else
				{
					break;
				}
			}
		}
	}
}

// adds all "physically" possible moves to "moves", including moves that are not legal in terms of check
// if checkCastling is false, this method does not check for castling moves, since otherwise checkMoves() and isCheck() could call each other endlessly
void King::checkMoves(const Board& board, bool checkCastling, vector<Move>& moves, const int* const position)
{
	const int x = position[0];
	const int y = position[1];
	for (int dx = -1; dx <= 1; ++dx)
	{
		for (int dy = -1; dy <= 1; ++dy)
		{
			if (!(dx == 0 && dy == 0) && x + dx >= 1 && x + dx <= 8 && y + dy >= 1 && y + dy <= 8)
			{
				// square is empty
				if (board.board[x + dx][y + dy] == nullptr)
				{
					moves.push_back(Move(x, y, x + dx, y + dy));
				}
				// square is occupied by enemy piece
				else if (color != board.board[x + dx][y + dy]->color)
				{
					moves.push_back(Move(x, y, x + dx, y + dy));
				}
			}
		}
	}
	// castling
	if (checkCastling)
	{
		if (x == 5 && ((y == 8 && board.castlingInformation[0]) || (y == 1 && board.castlingInformation[3])))
		{
			// rook A
			if ((board.castlingInformation[1] && color == board.board[1][8]->color)
				|| (board.castlingInformation[4] && color == board.board[1][1]->color))
			{
				if (board.board[2][y] == nullptr && board.board[3][y] == nullptr && board.board[4][y] == nullptr && !isCheck(board, color))
				{
					bool areAllSquaresNotCheck = true;
					for (int i = 3; i <= 4; i++)
					{
						Board tempBoard = board;
						movePiece(tempBoard, true, Move(x, y, i, y));
						if (isCheck(tempBoard, color))
						{
							areAllSquaresNotCheck = false;
							break;
						}
					}
					if (areAllSquaresNotCheck)
					{
						moves.push_back(Move(x, y, 3, y, 1));
					}
				}
			}
			// rook H
			if ((board.castlingInformation[2] && color == board.board[8][8]->color)
				|| (board.castlingInformation[5] && color == board.board[8][1]->color))
			{
				if (board.board[6][y] == nullptr && board.board[7][y] == nullptr && !isCheck(board, color))
				{
					bool areAllFieldsNotCheck = true;
					for (int i = 6; i <= 7; i++)
					{
						Board tempBoard = board;
						movePiece(tempBoard, true, Move(x, y, i, y));
						if (isCheck(tempBoard, color))
						{
							areAllFieldsNotCheck = false;
							break;
						}
					}
					if (areAllFieldsNotCheck)
					{
						moves.push_back(Move(x, y, 7, y, 1));
					}
				}
			}
		}
	}
}

// adds all "physically" possible moves to "moves", including moves that are not legal in terms of check
void Knight::checkMoves(const Board& board, bool checkCastling, vector<Move>& moves, const int* const position)
{
	const int x = position[0];
	const int y = position[1];
	const int dxArr[4] = { -2, -1, 1, 2 };
	for (int dx : dxArr)
	{
		if (dx == -2 || dx == 2)
		{
			for (int dy = -1; dy <= 1; dy += 2)
			{
				if (x + dx >= 1 && x + dx <= 8 && y + dy >= 1 && y + dy <= 8)
				{
					// square is empty
					if (board.board[x + dx][y + dy] == nullptr)
					{
						moves.push_back(Move(x, y, x + dx, y + dy));
					}
					// square is occupied by enemy piece
					else if (color != board.board[x + dx][y + dy]->color)
					{
						moves.push_back(Move(x, y, x + dx, y + dy));
					}
				}
			}
		}
		else
		{
			for (int dy = -2; dy <= 2; dy += 4)
			{
				if (x + dx >= 1 && x + dx <= 8 && y + dy >= 1 && y + dy <= 8)
				{
					// square is empty
					if (board.board[x + dx][y + dy] == nullptr)
					{
						moves.push_back(Move(x, y, x + dx, y + dy));
					}
					// square is occupied by enemy piece
					else if (color != board.board[x + dx][y + dy]->color)
					{
						moves.push_back(Move(x, y, x + dx, y + dy));
					}
				}
			}
		}
	}
}

// adds all "physically" possible moves to "moves", including moves that are not legal in terms of check
void Pawn::checkMoves(const Board& board, bool checkCastling, vector<Move>& moves, const int* const position)
{
	const int x = position[0];
	const int y = position[1];
	const int ax = board.latestMove[0];
	const int ay = board.latestMove[1];
	const int bx = board.latestMove[2];
	const int by = board.latestMove[3];
	// player
	if (isPlayer)
	{
		// diagonal capturing move
		for (int dx = -1; dx <= 1; dx += 2)
		{
			if (x + dx >= 1 && x + dx <= 8 && y + 1 <= 8 && board.board[x + dx][y + 1] != nullptr && board.board[x + dx][y + 1]->color != color)
			{
				if (y == 7)
				{
					// promotion
					moves.push_back(Move(x, y, x + dx, y + 1, 3));
				}
				else
				{
					// normal move
					moves.push_back(Move(x, y, x + dx, y + 1));
				}
			}
		}
		// straight move
		if (y + 1 <= 8
			&& board.board[x][y + 1] == nullptr
			&& (x == 1 || board.board[x - 1][y + 1] == nullptr || board.board[x - 1][y + 1]->color == color)
			&& (x == 8 || board.board[x + 1][y + 1] == nullptr || board.board[x + 1][y + 1]->color == color))
		{
			// one square
			if (y == 7)
			{
				// promotion
				moves.push_back(Move(x, y, x, y + 1, 3));
			}
			else
			{
				// normal move
				moves.push_back(Move(x, y, x, y + 1));
			}
			// two squares
			if (y == 2 && board.board[x][y + 2] == nullptr)
			{
				moves.push_back(Move(x, y, x, y + 2));
			}
		}
		// en passant
		if (y == 5
			&& typeid(*(board.board[bx][by])) == typeid(Pawn)
			&& ay == by + 2
			&& (bx == x - 1 || bx == x + 1))
		{
			moves.push_back(Move(x, y, bx, y + 1, 2));
		}
	}
	// computer
	else
	{
		// diagonal capturing move
		for (int dx = -1; dx <= 1; dx += 2)
		{
			if (x + dx >= 1 && x + dx <= 8 && y - 1 >= 1 && board.board[x + dx][y - 1] != nullptr && board.board[x + dx][y - 1]->color != color)
			{
				if (y == 2)
				{
					// promotion
					moves.push_back(Move(x, y, x + dx, y - 1, 3));
				}
				else
				{
					// normal move
					moves.push_back(Move(x, y, x + dx, y - 1));
				}
			}
		}
		// straight move
		if (y - 1 >= 1
			&& board.board[x][y - 1] == nullptr
			&& (x == 1 || board.board[x - 1][y - 1] == nullptr || board.board[x - 1][y - 1]->color == color)
			&& (x == 8 || board.board[x + 1][y - 1] == nullptr || board.board[x + 1][y - 1]->color == color))
		{
			// one square
			if (y == 2)
			{
				// promotion
				moves.push_back(Move(x, y, x, y - 1, 3));
			}
			else
			{
				// normal move
				moves.push_back(Move(x, y, x, y - 1));
			}
			// two squares
			if (y == 7 && board.board[x][y - 2] == nullptr)
			{
				moves.push_back(Move(x, y, x, y - 2));
			}
		}
		// en passant
		if (y == 4
			&& typeid(*(board.board[bx][by])) == typeid(Pawn)
			&& ay == by - 2
			&& (bx == x - 1 || bx == x + 1))
		{
			moves.push_back(Move(x, y, bx, y - 1, 2));
		}
	}
}

// adds all "physically" possible moves to "moves", including moves that are not legal in terms of check
void Queen::checkMoves(const Board& board, bool checkCastling, vector<Move>& moves, const int* const position)
{
	const int x = position[0];
	const int y = position[1];
	for (int dx = -1; dx <= 1; ++dx)
	{
		for (int dy = -1; dy <= 1; ++dy)
		{
			if (!(dx == 0 && dy == 0))
			{
				int i = 1;
				while (x + i * dx >= 1 && x + i * dx <= 8 && y + i * dy >= 1 && y + i * dy <= 8)
				{
					// next square is empty
					if (board.board[x + i * dx][y + i * dy] == nullptr)
					{
						moves.push_back(Move(x, y, x + i * dx, y + i * dy));
						++i;
					}
					// square is occupied by enemy piece
					else if (color != board.board[x + i * dx][y + i * dy]->color)
					{
						moves.push_back(Move(x, y, x + i * dx, y + i * dy));
						break;
					}
					// square is occupied by piece of own color
					else
					{
						break;
					}
				}
			}
		}
	}
}

// adds all "physically" possible moves to "moves", including moves that are not legal in terms of check
void Rook::checkMoves(const Board& board, bool checkCastling, vector<Move>& moves, const int* const position)
{
	const int x = position[0];
	const int y = position[1];
	// x-axis
	for (int dx = -1; dx <= 1; ++dx)
	{
		int i = 1;
		while (x + i * dx >= 1 && x + i * dx <= 8)
		{
			if (board.board[x + i * dx][y] == nullptr)
			{
				// square is empty
				moves.push_back(Move(x, y, x + i * dx, y));
				++i;
			}
			else if (color != board.board[x + i * dx][y]->color)
			{
				// square is occupied by enemy piece
				moves.push_back(Move(x, y, x + i * dx, y));
				break;
			}
			else
			{
				// square is occupied by piece of own color
				break;
			}
		}
	}
	// y-axis
	for (int dy = -1; dy <= 1; ++dy)
	{
		int i = 1;
		while (y + i * dy >= 1 && y + i * dy <= 8)
		{
			if (board.board[x][y + i * dy] == nullptr)
			{
				// square is empty
				moves.push_back(Move(x, y, x, y + i * dy));
				++i;
			}
			else if (color != board.board[x][y + i * dy]->color)
			{
				// square is occupied by enemy piece
				moves.push_back(Move(x, y, x, y + i * dy));
				break;
			}
			else
			{
				// square is occupied by piece of own color
				break;
			}
		}
	}
}