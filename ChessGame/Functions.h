#pragma once
#include <vector>
#include "Board.h"
#include "Move.h"
#include "Piece.h"

using std::vector;

extern const double RANGE_OF_MOVE_VALUES;
extern const double ITERATIONS;
extern const double HIGHEST_POSSIBLE_MOVE_VALUE;
extern const double LOWEST_POSSIBLE_MOVE_VALUE;
extern const double PAWN_VALUE;
extern const double BISHOP_VALUE;
extern const double KNIGHT_VALUE;
extern const double ROOK_VALUE;
extern const double QUEEN_VALUE;

vector<Move> checkMoves(const Board& board, const Color& movingColor);
int checkStatus(const Board& board, const Color& movingColor);
Move chooseMove(vector<Move>& moves);
bool isCheck(const Board& board, const Color& color);
int isCheckmateOrStalemate(const Board& board, const Color& movingColor);
void moveBlack(Board& board, const Color& playerColor);
void moveComputer(Board& board, const Color& playerColor);
void movePiece(Board& board, bool isTemp, const Move& move);
void movePlayer(Board& board, const Color& playerColor);
void moveWhite(Board& board, const Color& playerColor);
void play(const Color& playerColor);
int scoreBoard(const Board& board, const Color& playerColor);
void scoreMoves(const Board& board, const int iterations, vector<Move>& moves, const Color& movingColor, const Color& playerColor);