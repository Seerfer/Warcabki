#ifndef BOARD_HPP_ 
#define BOARD_HPP_

#include <vector>
#include <cmath>

using namespace std;

struct Coords 
{
    int x1:4;
    int y1:4;
    int x2:4;
    int y2:4;
    Coords (int x1_, int y1_, int x2_, int y2_);
    bool operator != (const Coords &B);
};

struct Move 
{
    std::vector< Coords > coords;
    Move(){}
    Move(int x1, int y1, int x2, int y2);
    Move(Coords c);
    void add(Coords c);
    bool operator== (const Move &B);
};


enum Pawns   // wszystkie możliwe stany pionów
{
    EMPTY,
    LIGHT_PAWN,
    DARK_PAWN,
    LIGHT_KING,
    DARK_KING
};


class Board
{
	public:

	Pawns board[8][8];
	int lightP, darkP, lightK, darkK;
	int drawCounter;  // jeśli przez 14 tur obu graczy nie zostanie zbity żaden pion, gra zakończy się remisem

	Board();
	void reset();
	bool checkCapture(int x, int y);
    bool checkCapture(bool color);

    std::vector <Move> getMoveSequences(Move move, Board b);  // zwraca wszystkie możliwości wykonania ruchu
    std::vector <Move> getViableMoves(bool color);
    std::vector <Move> getViableMoves(int x, int y);

    void capture(int x1, int y1, int x2, int y2);
    // zwróć 0 w przypadku błędnego ruchu, 1 gdy zakończono turę, 2 gdy pozostaje ruch do wykonania
    int move(Coords m);
    int move(Move m);
    int move(int x1, int y1, int x2, int y2, bool color);

    int checkState(bool color); // sprawdza stan gry (kontynuacja, zwycięśtwo ktorejś ze stron, remis)
    int evaluateField(int x, int y);
    int evaluateBoard(bool color, bool side);
};

#endif