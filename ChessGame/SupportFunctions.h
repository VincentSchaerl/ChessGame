#pragma once
#include <string>
#include "Board.h"
#include "Move.h"
#include "Piece.h"

using std::string;

string charToClassName(const char c);
int charToInt(const char c);
string colorToString(const Color& color);
char digitToLetter(const int i);
Color inputColor();
Move inputMove(const Board& board, const Color& playerColor);
string inputMoveString();
bool inputPlayAgain();
Piece* inputPromotion(const Color& playerColor);
bool isCharValidX(const char c);
bool isCharValidY(const char c);
int letterToDigit(const char c);
Color oppositeColor(const Color& color);
void printBoard(const Board& board);
void printInstructions(const Color& playerColor);
void printResult(const Color& playerColor, const int status);
string stringToLowerCase(string s);
Move stringToMove(const Board& board, const string& move);