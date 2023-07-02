#include "Functions.h"
#include <iostream>
#include "SupportFunctions.h"

using std::cin;
using std::cout;

const double RANGE_OF_MOVE_VALUES = 0.08;
// iterations = how many moves in advance get calculated
const double ITERATIONS = 3;
// the highest possible value of a move is if the resulting position includes:
// 2 bishops + 2 knights + 2 rooks + 9 queens + no enemy piece on board except the king =
// = 2*3 + 2*3 + 2*5 + 9*9 = 103
const double HIGHEST_POSSIBLE_MOVE_VALUE = 103;
// vice versa for the lowest possible value
const double LOWEST_POSSIBLE_MOVE_VALUE = -103;
// according to Wikipedia, this is the most common assignment of point values:
const double PAWN_VALUE = 1;
const double BISHOP_VALUE = 3;
const double KNIGHT_VALUE = 3;
const double ROOK_VALUE = 5;
const double QUEEN_VALUE = 9;

// returns all possible and legal moves of the moving color
vector<Move> checkMoves(const Board& board, const Color& movingColor)
{
	vector<Move> tempMoves;
	// adds all "physically" possible moves to "tempMoves"
	for (int i = 1; i <= 8; ++i)
	{
		for (int j = 1; j <= 8; ++j)
		{
			if (board.board[i][j] != nullptr && board.board[i][j]->color == movingColor)
			{
				const int position[2] = { i, j };
				board.board[i][j]->checkMoves(board, true, tempMoves, position);
			}
		}
	}
	vector<Move> moves;
	// checks which of these moves are legal in terms of check and adds them to "moves"
	for (vector<Move>::iterator it = tempMoves.begin(); it != tempMoves.end(); ++it)
	{
		Board tempBoard = board;
		movePiece(tempBoard, true, *it);
		if (!isCheck(tempBoard, movingColor))
		{
			moves.push_back(*it);
		}
		// if move (*it) is of type 3 (= promotion), the queen created by movePiece() has to be deleted
		if (it->type == 3)
		{
			delete tempBoard.board[it->bx][it->by];
		}
	}
	return moves;
}

// 0 = game goes on, 1 = black wins, 2 = white wins, 3 = remis
// "movingColor" is the color that makes the next move
int checkStatus(const Board& board, const Color& movingColor)
{
	if (movingColor == white && isCheckmateOrStalemate(board, white) == 1)
	{
		return 1;
	}
	if (movingColor == black && isCheckmateOrStalemate(board, black) == 1)
	{
		return 2;
	}
	if (isCheckmateOrStalemate(board, movingColor) == 2)
	{
		return 3;
	}
	return 0;
}

// if the highest value of all moves = HIGHEST_POSSIBLE_VALUE, i.e. it checkmates the player, this move gets chosen
// otherwise all moves with a value greater than or euqal the highest value - RANGE_OF_MOVE_VALUES get added to "bestMoves" and one of them gets randomly chosen
Move chooseMove(vector<Move>& moves)
{
	double highestValue = LOWEST_POSSIBLE_MOVE_VALUE;
	for (vector<Move>::iterator it = moves.begin(); it != moves.end(); ++it)
	{
		if (it->getValue() >= highestValue)
		{
			highestValue = it->getValue();
		}
	}
	double rangeOfMoveValues = 0;
	if (highestValue != HIGHEST_POSSIBLE_MOVE_VALUE)
	{
		rangeOfMoveValues = RANGE_OF_MOVE_VALUES;
	}
	vector<Move> bestMoves;
	for (vector<Move>::iterator it = moves.begin(); it != moves.end(); ++it)
	{
		if (it->getValue() >= highestValue - rangeOfMoveValues)
		{
			bestMoves.push_back(*it);
		}
	}
	return bestMoves[rand() % bestMoves.size()];
}

// checks if the king of the color "color" is in check
bool isCheck(const Board& board, const Color& color)
{
	vector<Move> enemyMoves;
	// adds all "physically" possible moves of the enemy color to "enemyMoves"
	for (int i = 1; i <= 8; ++i)
	{
		for (int j = 1; j <= 8; ++j)
		{
			if (board.board[i][j] != nullptr && board.board[i][j]->color != color)
			{
				const int position[2] = { i, j };
				// checkCastling = false, because otherwise checkMoves() and isCheck() could call each other endlessly
				board.board[i][j]->checkMoves(board, false, enemyMoves, position);
			}
		}
	}
	// checks if one of these moves would capture the king
	for (vector<Move>::iterator it = enemyMoves.begin(); it != enemyMoves.end(); ++it)
	{
		if (board.board[it->bx][it->by] != nullptr && typeid(*(board.board[it->bx][it->by])) == typeid(King))
		{
			return true;
		}
	}
	return false;
}

// 0 = neither checkmate nor stalemate, 1 = checkmate, 2 = stalemate
// "movingColor" is the color that makes the next move, i.e. the color of the king to be checked if it is checkmated or stalemated
int isCheckmateOrStalemate(const Board& board, const Color& movingColor)
{
	// moves = all possible and legal moves
	vector<Move> moves = checkMoves(board, movingColor);
	if (moves.size() == 0)
	{
		if (isCheck(board, movingColor))
		{
			return 1;
		}
		return 2;
	}
	return 0;
}

void moveBlack(Board& board, const Color& playerColor)
{
	if (playerColor == black)
	{
		movePlayer(board, playerColor);
	}
	else
	{
		moveComputer(board, playerColor);
	}
}

void moveComputer(Board& board, const Color& playerColor)
{
	const Color computerColor = oppositeColor(playerColor);
	cout << "Computer calculates...\n\n";
	vector<Move> moves = checkMoves(board, computerColor);
	scoreMoves(board, ITERATIONS, moves, computerColor, playerColor);
	Move move = chooseMove(moves);
	const int ax = move.ax;
	const int ay = move.ay;
	const int bx = move.bx;
	const int by = move.by;
	cout << "Computer: " << colorToString(computerColor) << " " << charToClassName(board.board[ax][ay]->getChar()) << " from " << digitToLetter(ax)
		<< ay << " to " << digitToLetter(bx) << by;
	// castling
	if (move.type == 1)
	{
		cout << " (castling)";
	}
	// en passant
	else if (move.type == 2)
	{
		cout << " (en passant)";
	}
	// capture
	if (board.board[bx][by] != nullptr && board.board[bx][by]->color == playerColor)
	{
		cout << ", capturing " << stringToLowerCase(colorToString(playerColor)) << " " << charToClassName(board.board[bx][by]->getChar());
	}
	// promotion
	if (board.board[ax][ay] != nullptr && typeid(*(board.board[ax][ay])) == typeid(Pawn) && by == 1)
	{
		cout << "\n" << colorToString(computerColor) << " Pawn was promoted to Queen.";
	}
	cout << "\n\n";
	movePiece(board, false, move);
}

void movePiece(Board& board, bool isTemp, const Move& move)
{
	const int ax = move.ax;
	const int ay = move.ay;
	const int bx = move.bx;
	const int by = move.by;
	// normal move
	if (move.type == 0)
	{
		if (!isTemp && board.board[bx][by] != nullptr)
		{
			delete board.board[bx][by];
		}
		board.board[bx][by] = board.board[ax][ay];
		board.board[ax][ay] = nullptr;
	}
	// castling
	else if (move.type == 1)
	{
		board.board[bx][by] = board.board[ax][ay];
		board.board[ax][ay] = nullptr;
		// rook A
		if (bx == 3)
		{
			// isTemp = true because there is no possibility of capture (-> delete) or promotion (-> new) and movePiece() gets also called if calling function movePiece() got called with isTremp = true
			movePiece(board, true, Move(1, by, 4, by));
		}
		// rook H
		else
		{
			// isTemp = true because there is no possibility of capture (-> delete) or promotion (-> new) and movePiece() gets also called if calling function movePiece() got called with isTremp = true
			movePiece(board, true, Move(8, by, 6, by));
		}
	}
	// en passant
	else if (move.type == 2)
	{
		board.board[bx][by] = board.board[ax][ay];
		board.board[ax][ay] = nullptr;
		if (!isTemp)
		{
			delete board.board[bx][ay];
		}
		board.board[bx][ay] = nullptr;
	}
	// promotion (type == 3)
	else
	{
		Color color = board.board[ax][ay]->color;
		// temporary
		if (isTemp)
		{
			board.board[ax][ay] = nullptr;
			board.board[bx][by] = new Queen(color);
		}
		// not temporary
		else
		{
			delete board.board[ax][ay];
			board.board[ax][ay] = nullptr;
			if (board.board[bx][by] != nullptr)
			{
				delete board.board[bx][by];
			}
			// computer
			if (by == 1)
			{
				board.board[bx][by] = new Queen(color);
			}
			// player
			else
			{
				Piece* promotionPiece = inputPromotion(color);
				board.board[bx][by] = promotionPiece;
			}
		}
	}
	// castling information
	if (board.castlingInformation[0] == true && ax == 5 && ay == 8)
	{
		board.castlingInformation[0] = false;
	}
	else if (board.castlingInformation[1] == true && ax == 1 && ay == 8)
	{
		board.castlingInformation[1] = false;
	}
	else if (board.castlingInformation[2] == true && ax == 8 && ay == 8)
	{
		board.castlingInformation[2] = false;
	}
	else if (board.castlingInformation[3] == true && ax == 5 && ay == 1)
	{
		board.castlingInformation[3] = false;
	}
	else if (board.castlingInformation[4] == true && ax == 1 && ay == 1)
	{
		board.castlingInformation[4] = false;
	}
	else if (board.castlingInformation[5] == true && ax == 8 && ay == 1)
	{
		board.castlingInformation[5] = false;
	}
	// latestMove
	board.setLatestMove(move);
}

void movePlayer(Board& board, const Color& playerColor)
{
	Move move = inputMove(board, playerColor);
	const int ax = move.ax;
	const int ay = move.ay;
	const int bx = move.bx;
	const int by = move.by;
	cout << "Player: " << colorToString(playerColor) << " " << charToClassName(board.board[ax][ay]->getChar()) << " from " << digitToLetter(ax)
		<< ay << " to " << digitToLetter(bx) << by;
	// castling
	if (move.type == 1)
	{
		cout << " (castling)";
	}
	// en passant
	else if (move.type == 2)
	{
		cout << " (en passant)";
	}
	// capture
	if (board.board[bx][by] != nullptr && board.board[bx][by]->color == oppositeColor(playerColor))
	{
		cout << ", capturing " << stringToLowerCase(colorToString(oppositeColor(playerColor))) << " " << charToClassName(board.board[bx][by]->getChar());
	}
	cout << "\n\n";
	// promotion: movePiece() -> inputPromotion()
	movePiece(board, false, move);
}

void moveWhite(Board& board, const Color& playerColor)
{
	if (playerColor == black)
	{
		moveComputer(board, playerColor);
	}
	else
	{
		movePlayer(board, playerColor);
	}
}

// players pieces always occupy the squares A1 - B8
void play(const Color& playerColor)
{
	printInstructions(playerColor);
	Board board(playerColor);
	printBoard(board);
	// 0 = game goes on, 1 = black wins, 2 = white wins, 3 = remis
	int status = 0;
	while (true)
	{
		status = checkStatus(board, white);
		if (status != 0)
		{
			break;
		}
		moveWhite(board, playerColor);
		printBoard(board);
		status = checkStatus(board, black);
		if (status != 0)
		{
			break;
		}
		moveBlack(board, playerColor);
		printBoard(board);
	}
	printResult(playerColor, status);
	board.clear();
}

// scores the board in favor of the computer: pieces of the player deduct points, pieces of the computer add points
int scoreBoard(const Board& board, const Color& playerColor)
{
	int points = 0;
	for (int i = 1; i <= 8; ++i)
	{
		for (int j = 1; j <= 8; ++j)
		{
			if (board.board[i][j] != nullptr)
			{
				char c = tolower(board.board[i][j]->getChar());
				if (board.board[i][j]->color == playerColor)
				{
					switch (c)
					{
					case 'p': points -= PAWN_VALUE; break;
					case 'b': points -= BISHOP_VALUE; break;
					case 'n': points -= KNIGHT_VALUE; break;
					case 'r': points -= ROOK_VALUE; break;
					case 'q': points -= QUEEN_VALUE; break;
					}
				}
				else
				{
					switch (c)
					{
					case 'p': points += PAWN_VALUE; break;
					case 'b': points += BISHOP_VALUE; break;
					case 'n': points += KNIGHT_VALUE; break;
					case 'r': points += ROOK_VALUE; break;
					case 'q': points += QUEEN_VALUE; break;
					}
				}
			}
		}
	}
	return points;
}

// assigns a value to every move of "moves"
// "movingColor" is the color that makes the next move (one of vector "moves")
void scoreMoves(const Board& board, const int iterations, vector<Move>& moves, const Color& movingColor, const Color& playerColor)
{
	for (vector<Move>::iterator it = moves.begin(); it != moves.end(); ++it)
	{
		Board tempBoard = board;
		movePiece(tempBoard, true, *it);
		int tempStatus = checkStatus(tempBoard, oppositeColor(movingColor));
		// game goes on:
		if (tempStatus == 0)
		{
			if (iterations == 1)
			{
				it->setValue(scoreBoard(tempBoard, playerColor));
			}
			else
			{
				vector<Move> tempMoves = checkMoves(tempBoard, oppositeColor(movingColor));
				scoreMoves(tempBoard, iterations - 1, tempMoves, oppositeColor(movingColor), playerColor);
				double points = 0;
				for (vector<Move>::iterator it = tempMoves.begin(); it != tempMoves.end(); ++it)
				{
					points += it->getValue();
				}
				double value = points / tempMoves.size();
				it->setValue(value);
			}
		}
		// computer wins:
		else if ((playerColor == black && tempStatus == 2) || (playerColor == white && tempStatus == 1))
		{
			it->setValue(HIGHEST_POSSIBLE_MOVE_VALUE);
		}
		// player wins:
		else if ((playerColor == black && tempStatus == 1) || (playerColor == white && tempStatus == 2))
		{
			it->setValue(LOWEST_POSSIBLE_MOVE_VALUE);
		}
		// remis:
		else
		{
			it->setValue(0);
		}
		// if move (*it) is of type 3 (= promotion), the queen created by movePiece() has to be deleted
		if (it->type == 3)
		{
			delete tempBoard.board[it->bx][it->by];
		}
	}
}