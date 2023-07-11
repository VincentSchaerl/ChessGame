#include "SupportFunctions.h"
#include <iostream>
#include "Functions.h"

using std::cin;
using std::cout;

string charToClassName(const char c)
{
	const char cl = tolower(c);
	switch (cl)
	{
	case 'b': return "Bishop";
	case 'k': return "King";
	case 'n': return "Night";
	case 'p': return "Pawn";
	case 'q': return "Queen";
	case 'r': return "Rook";
	}
}

int charToInt(const char c)
{
	switch (c)
	{
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	}
}

string colorToString(const Color& color)
{
	if (color == black)
	{
		return "Black";
	}
	else
	{
		return "White";
	}
}

char digitToLetter(const int i)
{
	switch (i)
	{
	case 1: return 'A';
	case 2: return 'B';
	case 3: return 'C';
	case 4: return 'D';
	case 5: return 'E';
	case 6: return 'F';
	case 7: return 'G';
	case 8: return 'H';
	}
}

Color inputColor()
{
	string input;
	cout << "Which color do you want to play (b for black, w for white)?:\n\n\n";
	getline(cin, input);
	cout << "\n\n";
	if (input == "b" || input == "B")
	{
		return black;
	}
	else if (input == "w" || input == "W")
	{
		return white;
	}
	cout << "Invalid input.\n";
	return inputColor();
}

Move inputMove(const Board& board, const Color& playerColor)
{
	const string input = inputMoveString();
	const Move move = stringToMove(board, input);
	vector<Move> moves = checkMoves(board, playerColor);
	for (vector<Move>::iterator it = moves.begin(); it != moves.end(); ++it)
	{
		if (*it == move)
		{
			return *it;
		}
	}
	cout << "This move is not possible.\n";
	return inputMove(board, playerColor);
}

string inputMoveString()
{
	string input;
	cout << "Please declare your move:\n\n\n";
	getline(cin, input);
	cout << "\n\n";
	input = stringToLowerCase(input);
	if (input.length() != 5 || !isCharValidX(input[0]) || !isCharValidY(input[1]) || input[2] != ' ' || !isCharValidX(input[3]) || !isCharValidY(input[4]))
	{
		cout << "Invalid input.\n";
		return inputMoveString();
	}
	return input;
}

bool inputPlayAgain()
{
	string input;
	cout << "Do you want to play again? (0 for no, 1 for yes)\n\n\n";
	getline(cin, input);
	cout << "\n\n";
	if (input == "0" || input == "1")
	{
		return stoi(input);
	}
	cout << "Invalid input.\n";
	return inputPlayAgain();
}

Piece* inputPromotion(const Color& playerColor)
{
	string input;
	cout << "Which piece shall the Pawn be promoted to (b for Bishop, n for Knight, q for Queen, r for Rook)?\n\n\n";
	getline(cin, input);
	cout << "\n\n";
	if (input == "b" || input == "B")
	{
		cout << colorToString(playerColor) << " Pawn was promoted to Bishop.\n\n";
		return new Bishop(playerColor);
	}
	if (input == "n" || input == "N")
	{
		cout << colorToString(playerColor) << " Pawn was promoted to Knight.\n\n";
		return new Knight(playerColor);
	}
	if (input == "q" || input == "Q")
	{
		cout << colorToString(playerColor) << " Pawn Pawn was promoted to Queen.\n\n";
		return new Queen(playerColor);
	}
	if (input == "r" || input == "R")
	{
		cout << colorToString(playerColor) << " Pawn was promoted to Rook.\n\n";
		return new Rook(playerColor);
	}
	cout << "Invalid input.\n";
	return inputPromotion(playerColor);
}

bool isCharValidX(const char c)
{
	if (c != 'a' && c != 'b' && c != 'c' && c != 'd' && c != 'e' && c != 'f' && c != 'g' && c != 'h')
	{
		return false;
	}
	return true;
}

bool isCharValidY(const char c)
{
	if (c != '1' && c != '2' && c != '3' && c != '4' && c != '5' && c != '6' && c != '7' && c != '8')
	{
		return false;
	}
	return true;
}

int letterToDigit(const char c)
{
	switch (c)
	{
	case 'a': return 1;
	case 'b': return 2;
	case 'c': return 3;
	case 'd': return 4;
	case 'e': return 5;
	case 'f': return 6;
	case 'g': return 7;
	case 'h': return 8;
	}
}

Color oppositeColor(const Color& color)
{
	if (color == black)
	{
		return white;
	}
	return black;
}

void printBoard(const Board& board)
{
	cout << "\n\n";
	cout << "     A   B   C   D   E   F   G   H     \n";
	cout << "  -|---|---|---|---|---|---|---|---|-  \n";
	for (int i = 16; i >= 1; --i)
	{
		if (i % 2 == 0)
		{
			int y = i / 2;
			cout << " " << y << " |";
			for (int j = 1; j <= 8; ++j)
			{
				int x = j;
				cout << " ";
				if (board.board[x][y] == nullptr)
				{
					cout << " ";
				}
				else
				{
					cout << board.board[x][y]->getChar();
				}
				cout << " |";
			}
			cout << " " << y;
		}
		else
		{
			cout << "  -|---|---|---|---|---|---|---|---|-  ";
		}
		cout << "\n";
	}
	cout << "     A   B   C   D   E   F   G   H     \n";
	cout << "\n\n\n";
}

void printInstructions(const Color& playerColor)
{
	if (playerColor == black)
	{
		cout << "White pieces (computer):    \"B\" = Bishop, \"K\" = King, \"N\" = Night, \"P\" = Pawn, \"Q\" = Queen, \"R\" = Rook    <- upper case\n";
		cout << "Black pieces (player):      \"b\" = Bishop, \"k\" = King, \"n\" = Night, \"p\" = Pawn, \"q\" = Queen, \"r\" = Rook    <- lower case\n\n";
	}
	else
	{
		cout << "Black pieces (computer):    \"b\" = Bishop, \"k\" = King, \"n\" = Night, \"p\" = Pawn, \"q\" = Queen, \"r\" = Rook    <- lower case\n";
		cout << "White pieces (player):      \"B\" = Bishop, \"K\" = King, \"N\" = Night, \"P\" = Pawn, \"Q\" = Queen, \"R\" = Rook    <- upper case\n\n";
	}
	cout << "Declare a move as follows: \"[position of piece to be moved] [position of destination]\"\n";
	cout << "Examples: \"e2 e4\", \"B1 C3\"\n\n";
}

void printResult(const Color& playerColor, const int status)
{
	switch (status)
	{
	case 1: cout << "Black wins!"; break;
	case 2: cout << "White wins!"; break;
	case 3: cout << "Remis!"; break;
	}
	if ((playerColor == black && status == 1) || (playerColor == white && status == 2))
	{
		cout << " Congratulations.";
	}
	cout << "\n\n";
}

string stringToLowerCase(string s)
{
	for (int i = 0; i < s.length(); ++i)
	{
		s[i] = tolower(s[i]);
	}
	return s;
}

Move stringToMove(const Board& board, const string& move)
{
	const int ax = letterToDigit(move[0]);
	const int ay = charToInt((move[1]));
	const int bx = letterToDigit(move[3]);
	const int by = charToInt((move[4]));
	return Move(ax, ay, bx, by);
}