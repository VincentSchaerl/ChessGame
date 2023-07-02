#include "Move.h"

// ax = x-coordinate of original position, ay == y-coordinate of original position, bx = x-coordinate of destination, by = y-coordinate of destination
// type: 0 = normal move, 1 = castling, 2 = en passant, 3 = promotion

Move::Move(const int ax, const int ay, const int bx, const int by) : ax(ax), ay(ay), bx(bx), by(by), type(0), value(0) {}

Move::Move(const int ax, const int ay, const int bx, const int by, const int type) : ax(ax), ay(ay), bx(bx), by(by), type(type), value(0) {}

bool operator==(const Move& move1, const Move& move2)
{
	return (move1.ax == move2.ax && move1.ay == move2.ay && move1.bx == move2.bx && move1.by == move2.by);
}

bool operator!=(const Move& move1, const Move& move2)
{
	return (move1.ax != move2.ax || move1.ay != move2.ay || move1.bx != move2.bx || move1.by != move2.by);
}

double Move::getValue()
{
	return value;
}

void Move::setValue(const double value)
{
	this->value = value;
}