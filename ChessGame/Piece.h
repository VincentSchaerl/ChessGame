#pragma once
#include <vector>
#include "Move.h"

using std::vector;

class Board;

enum Color
{
	black, white
};

class Piece
{
public:
// variables
	const Color color;
// functions
	Piece(const Color& color);
	virtual char getChar() = 0;
	virtual void checkMoves(const Board& board, bool checkCastling, vector<Move>& moves, const int* const position) = 0;
};

class Bishop : public Piece
{
public:
	Bishop(const Color& color);
	char getChar();
	void checkMoves(const Board& board, bool checkCastling, vector<Move>& moves, const int* const position);
};

class King : public Piece
{
public:
	King(const Color& color);
	char getChar();
	void checkMoves(const Board& board, bool checkCastling, vector<Move>& moves, const int* const position);
};

class Knight : public Piece
{
public:
	Knight(const Color& color);
	char getChar();
	void checkMoves(const Board& board, bool checkCastling, vector<Move>& moves, const int* const position);
};

class Pawn : public Piece
{
	bool isPlayer;
public:
	Pawn(const Color& color, bool isPlayer);
	char getChar();
	void checkMoves(const Board& board, bool checkCastling, vector<Move>& moves, const int* const position);
};

class Queen : public Piece
{
public:
	Queen(const Color& color);
	char getChar();
	void checkMoves(const Board& board, bool checkCastling, vector<Move>& moves, const int* const position);
};

class Rook : public Piece
{
public:
	Rook(const Color& color);
	char getChar();
	void checkMoves(const Board& board, bool checkCastling, vector<Move>& moves, const int* const position);
};