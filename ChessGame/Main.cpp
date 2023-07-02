#include <ctime>
#include "Functions.h"
#include "SupportFunctions.h"

int main()
{
	srand(time(0));
	bool playAgain = true;
	while (playAgain)
	{
		Color playerColor = inputColor();
		play(playerColor);
		playAgain = inputPlayAgain();
	}
}