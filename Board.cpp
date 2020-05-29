#include "Board.hpp"

// KOORDYNATY

Coords::Coords(int x1_, int y1_, int x2_, int y2_) 
{
    x1 = x1_;
    y1 = y1_;
    x2 = x2_;
    y2 = y2_;
}

bool Coords::operator != (const Coords &B) 
{
    if (this -> x1 != B.x1 || this -> y1 != B.y1 || this -> x2 != B.x2 || this -> y2 != B.y2)
        return true;

    return false;
}


// RUCH
Move::Move(int x1, int y1, int x2, int y2) {coords.emplace_back(x1, y1, x2, y2);}
Move::Move(Coords c) { coords.push_back(c);}
void Move::add(Coords c) {coords.insert(coords.begin(), c);}

bool Move::operator == (const Move &B) 
{
    if (this -> coords.size() != B.coords.size())
        return false;

    else
        for (int i=0; i < coords.size(); i++)
        { 
            if (this -> coords[i] != B.coords[i])
                return false;
        }

    return true;
}


// PLANSZA
Board::Board() {reset();}

void Board::reset() 
{
    drawCounter == 0;
    for(int i=0; i<8; i++) 
    {
        for ( int j=0; j<8; j++) 
        {
            if (i%2 != j%2) 
            {
                if(i<3)
                    board[i][j] = LIGHT_PAWN;
                else if ( i > 4 )
                    board[i][j] = DARK_PAWN;
                else
                    board[i][j] = EMPTY; 
            } 

            else 
                board[i][j] = EMPTY;
        }
    }
}


// sprawdzenie możliwości bicia dla pionów
bool Board::checkCapture(int x, int y) 
{
	// sprawdzenie koloru
    bool color = ( board[x][y] == LIGHT_PAWN || board[x][y] == LIGHT_KING ) ? 0:1;  // jak 0 to jasny pion, jak 1 to ciemny
    
    // sprawdzenie pionów
    if (board[x][y] == LIGHT_PAWN || board[x][y] == DARK_PAWN) 
    {
        for(int a : {-1, 1}) //////////////////////////////////////////////////////////////
        {
            for(int b : {-1, 1}) 
            {
                int i = x + a;
                int j = y + b;
                if (i+a < 8 && i+a >= 0 && j+b < 8 && j+b >= 0) 
                {
                    if(color) 
                    {
                        if(board[i][j] == LIGHT_KING || board[i][j] == LIGHT_PAWN)
                            if ( board[i+a][j+b] == EMPTY )
                                return true;
                    } 
                    else 
                    {
                        if(board[i][j] == DARK_KING || board[i][j] == DARK_PAWN)
                        {
                            if(board[i+a][j+b] == EMPTY)
                                return true;
                        }
                    }
                }
            }
        }
    }
    
    // sprawdzenie, czy są królewskie piony 
    else if(board[x][y] == LIGHT_KING || board[x][y] == DARK_KING)
    {
        for(int a : {-1, 1})  //////////////////////////////////////////////////////////////
        {
            for (int b : {-1, 1}) 
            {
                int i = x + a;
                int j = y + b;
                while(i+a < 8 && i+a >= 0 && j+b < 8 && j+b >= 0) 
                {
                    if(color) 
                    {
                        if(board[i][j] == DARK_KING || board[i][j] == DARK_PAWN)
                            break;

                        if(board[i][j] == LIGHT_KING || board[i][j] == LIGHT_PAWN) 
                        {
                            if(board[i+a][j+b] == EMPTY) return true;
                            
                            else 
                            	break;
                        }

                    } 
                    
                    else 
                    {
                        if(board[i][j] == LIGHT_KING || board[i][j] == LIGHT_PAWN)
                            break;

                        if ( board[i][j] == DARK_KING || board[i][j] == DARK_PAWN ) 
                        {
                            if(board[i+a][j+b] == EMPTY)
                                return true;

                            else
                                break;
                        }
                    }
                    i += a;
                    j += b;
                }
            }
        }
    }
    return false;
}


// sprawdzenie możliwości bicia dla koloru
bool Board::checkCapture(bool color) 
{
    for(int i = 0; i < 8; i++) 
    {
        for (int j = 0; j < 8; j++) 
        {
            if (color) 
            {
                if (board[i][j] == DARK_KING || board[i][j] == DARK_PAWN)
                {
                    if (checkCapture(i, j))
                        return true;
                }
            } 
            else 
            {
                if(board[i][j] == LIGHT_KING || board[i][j] == LIGHT_PAWN)
                {
                    if(checkCapture(i, j))
                        return true;
                }
            }
        }
    }

    return false;
} 


// sprawdzenie wszystkich możliwych sekwencji po pierwszym ruchu
std::vector <Move> Board::getMoveSequences(Move move, Board b) 
{
    std::vector <Move> moves;
    if (b.move(move.coords[0]) == 1) 
        moves.emplace_back(move.coords[0]);
     
    else 
    {
        for (auto& m : b.getViableMoves(move.coords[0].x2, move.coords[0].y2)) 
        {
            std::vector <Move> tmp = getMoveSequences(m, b);
            moves.insert(moves.end(), tmp.begin(), tmp.end());
        }
        
        for (auto& m : moves) 
            m.add(move.coords[0]);
        
        return moves;
    }            
}


// sprawdzenie wszystkich możliwości pierwszego ruchu dla koloru
std::vector <Move> Board::getViableMoves(bool color) 
{
    std::vector< Move > moves;
    for (int i = 0; i < 8; i++) 
    {
        for (int j=0; j<8; j++) 
        {
            if (color) 
            {
                if (board[i][j] == DARK_KING || board[i][j] == DARK_PAWN) 
                {
                    auto tmp = getViableMoves(i, j);
                    for (auto& m : tmp) 
                    {
                        auto tmp2 = getMoveSequences(m, *this); 
                        moves.insert(moves.end(), tmp2.begin(), tmp2.end());
                    }
                }
            } 

            else 
            {
                if ( board[i][j] == LIGHT_KING || board[i][j] == LIGHT_PAWN ) 
                {
                    auto tmp = getViableMoves(i, j);
                    for (auto& m : tmp) 
                    {
                        auto tmp2 = getMoveSequences(m, *this); 
                        moves.insert(moves.end(), tmp2.begin(), tmp2.end());
                    }
                }
            }
        }
    }
    return moves;
}



// znalezienie wszystkich możliwości pierwszego ruchu dla pionu
std::vector <Move> Board::getViableMoves(int x, int y)
{
    bool color;
    std::vector <Move> moves;

    if (board[x][y] == EMPTY)  // jeśli pole jest puste
        return moves;            // zwróć pusty wektor

    if ( board[x][y] == LIGHT_PAWN || board[x][y] == LIGHT_KING) 
        color = 0;
     
    else 
        color = 1;
    
  
    if (checkCapture(color) != checkCapture(x, y))  // jeśli w kolejnym ruchu bicie jest możliwe, zwróć pusty wektor
        return moves;
    
    if (board[x][y] == LIGHT_PAWN || board[x][y] == DARK_PAWN)
    {
        
        if (!checkCapture(x, y)) // jeśli nie ma możliwości zbicia
        {
            int xdir = color ? -1 : 1;
            if (y < 7)
                if (board[x+xdir][y+1] == EMPTY)
                    moves.push_back(Move(x, y, x+xdir, y+1));

            if ( y > 0 )
            {
                if (board[x+xdir][y-1] == EMPTY)
                    moves.push_back( Move(x, y, x+xdir, y-1));
            }
        
        // If there are captures available
        } 
        else 
        {
            for (int a : {-1, 1})
            {
                for (int b : {-1, 1})
                {
                    int i = x + a;
                    int j = y + b;
                    if (i+a < 8 && i+a >= 0 && j+b < 8 && j+b >= 0) 
                    {
                        if (color)
                         {
                            if (board[i][j] == LIGHT_KING || board[i][j] == LIGHT_PAWN)
                            {
                                if (board[i+a][j+b] == EMPTY)
                                    moves.push_back(Move(x, y, i+a, j+b));
                            }
                        } 

                        else 
                        {
                            if (board[i][j] == DARK_KING || board[i][j] == DARK_PAWN)
                            {
                                if (board[i+a][j+b] == EMPTY)
                                    moves.push_back(Move(x, y, i+a, j+b));
                            }
                        }
                    }
                }
            }
        }
    } 

    else 
    {
        // flaguj jeśli ruch jest biciem
        bool captureFlag = checkCapture(x, y);
        
        // dla każdego kierunku
        for (int a : {-1, 1}) 
        {
            for (int b : {-1, 1}) 
            {
                int i = x + a;
                int j = y + b;
               
                // ruch dopóki nie pojawi się przeszkoda
                while (i >= 0 && j >= 0 && i <= 7 && j <= 7) 
                {
                    // jeśli nie ma bicia, dodaj te ruchy
                    if (board[i][j] == EMPTY) 
                    {
                        if (!captureFlag)
                            moves.push_back(Move(x, y, i, j));

                    } 
                    else if (color) 
                    {
                        // przerwij w przypadku pojawienia się przeciwnika na drodze
                        if ( board[i][j] == DARK_KING || board[i][j] == DARK_PAWN)
                            break;

                        // wszystkie możliwe ruchy po zbiciu
                        else if ((board[i][j] == LIGHT_KING || board[i][j] == LIGHT_PAWN) && captureFlag) 
                        {
                            do 
                            {
                                i += a;
                                j += b;
                                if (i >= 0 && j >= 0 && i <= 7 && j <= 7) 
                                {
                                    if (board[i][j] == EMPTY)
                                        moves.push_back(Move(x, y, i, j));

                                    else
                                        break;
                                } 
                                else
                                    break;
                            } while (true);
                            break;
                        } 
                        else
                            break;
                    } 
                    else 
                    {
                        if (board[i][j] == LIGHT_KING || board[i][j] == LIGHT_PAWN)
                            break;
                        
                        if ((board[i][j] == DARK_KING || board[i][j] == DARK_PAWN) && captureFlag) 
                        {
                            do 
                            {
                                i += a;
                                j += b;
                                if (i >= 0 && j >= 0 && i <= 7 && j <= 7) 
                                {
                                    if (board[i][j] == EMPTY)
                                        moves.push_back(Move(x, y, i, j));
                                    else
                                        break;
                                } 
                                else
                                    break;
                            } while (true);
                            break;
                        } 
                        else
                            break; 
                    }
                    i += a;
                    j += b;
                }
            }
        }
    }
    return moves;
}



// zbij pion lub króla z tych koordynatów
void Board::capture(int x1, int y1, int x2, int y2) 
{
    // oznacz kierunek
    int a = (x1<x2)? 1 : -1;
    int b = (y1<y2)? 1 : -1;
    
    for ( int i = x1+a, j = y1+b; i != x2, j != y2; i+=a, j+=b )
        board[i][j] = EMPTY; // wyczyść
}

// zwraca 0 - błędny ruch, 1 - zakończony ruch, 2 - pozostała możliwość zbicia do wykonania
int Board::move(Coords m) 
{
    bool color = (board[m.x1][m.y1] == LIGHT_KING || board[m.x1][m.y1] == LIGHT_PAWN) ? 0 : 1;
    return move(m.x1, m.y1, m.x2, m.y2, color);
}

int Board::move(Move m) 
{
    bool color = (board[m.coords[0].x1][m.coords[0].y1] == LIGHT_KING || board[m.coords[0].x1][m.coords[0].y1] == LIGHT_PAWN) ? 0 : 1;
    int i{};
    
    while (move(m.coords[i] ) == 2)
        i++;

    return 1;
}

int Board::move(int x1, int y1, int x2, int y2, bool color) 
{
    // sprawdzenie, czy gracz poruszył swoim kolorem
    if ((color && (board[x1][y1] == LIGHT_KING || board[x1][y1] == LIGHT_PAWN)) || (!color && (board[x1][y1] == DARK_KING || board[x1][y1] == DARK_PAWN)))
        return 0;

    bool captureFlag = false;

    for (auto& move : getViableMoves(x1, y1)) 
    {
        if ( move == Move( x1, y1, x2, y2 ) ) 
        {
            
            if (checkCapture(color))  // jeśli ruch zawiera zbicie
            {
                capture(x1, y1, x2, y2);
                captureFlag = true;
            }
            board[x2][y2] = board[x1][y1];
            board[x1][y1] = EMPTY;
            
            if (captureFlag && checkCapture(x2, y2)) // jeśli nadal można wykonać bicie
                return 2;
           
            else if ((!color && x2 == 7) || (color && x2 == 0))  // jeśli ruch zakończył się promocją
                board[x2][y2] = color ? DARK_KING : LIGHT_KING;
            return 1;
        }
    }
    return 0;
}


/* sprawdzenie wyniku
0 - gra toczy się dalej
1 - wygrały jasne
2 - wygrały ciemne
-1 - remis */
int Board::checkState(bool color) 
{
    int lightPawns{}, darkPawns{}, lightKings{}, darkKings{};
    bool lightMoves{}, darkMoves{};
    for (int i = 0; i < 8; i++) 
    {
        for (int j = 0; j < 8; j++) 
        {
            switch (board[i][j]) 
            {
                case EMPTY:
                    break;
                
                case LIGHT_PAWN:
                    if (!lightMoves)
                    {
                        if (getViableMoves( i, j ).size() != 0)
                            lightMoves = true;
                    }
                    lightPawns++;
                    break;

                case DARK_PAWN:
                    if (!darkMoves)
                            if (getViableMoves(i, j).size() != 0)
                                darkMoves = true;
                    darkPawns++;
                    break;

                case LIGHT_KING:
                    if (!lightMoves)
                    {
                        if (getViableMoves(i, j).size() != 0)
                            lightMoves = true;
                    }
                    lightKings++;
                    break;

                case DARK_KING:
                    if (!darkMoves)
                    {
                        if (getViableMoves(i, j).size() != 0)
                            darkMoves = true;
                    }
                    darkKings++;
                    break;
            }
        }
    }

    if ((darkPawns || darkKings) && (lightPawns || lightKings) && ((color && darkMoves) || (!color && lightMoves))) 
    {
        if (darkKings && lightKings && darkPawns == darkP && lightPawns == lightP && darkKings == darkK && lightKings == lightK) 
        {
            if (++drawCounter == 14) // jeśli przez 14 ruchów nie zostanie zbity żaden pion
                return -1;           // rozgrywka zakończy się remisem
            else
                drawCounter = 0;
        }

        darkP = darkPawns;
        lightP = lightPawns;
        darkK = darkKings;
        lightK  = lightKings;
        return 0;
    }

    else if ((!darkPawns && !darkKings) || (color && !darkMoves))
        return 1;

    else if ((!lightPawns && !lightKings) || (!color && !lightMoves)) 
        return 2;
}

/* FUNKCJA HEURYSTYCZNA */
int Board::evaluateField(int x, int y) 
{
    int score{};
    switch(board[x][y]) 
    {
        case EMPTY:
            break;

        case LIGHT_PAWN:
            score += 7;  // +7pkt za każdy pion
            if (x > 0) 
            {
                score += 2; // +2pkt jeśli pion jest w dalszym niż pierwszym rzędzie
                if (x > 1) 
                {
                    score ++;

                    if (x > 2)
                        score ++;; // +1pkt jeśli pion jest w dalszym niż drugim rzędzie
                }
            }
            if (y<2 || y>5)
            {
                score++;  // +1pkt jeśli pion jest jedno pole od ściany

                if (y<1 || y>6)
                    score++;  // +1pkt jeśli pion jest przy ścianie
            }
            score += 15;  // +15pkt jeśli pion ma możliwość bicia
            break;

            // dla ciemnych tak samo
        case DARK_PAWN:
            score += 7;
            if (x < 7) 
            {
                score += 2;
                if (x < 6) 
                {
                    score ++;
                    if (x < 5)
                        score++;
                }
            }

            if (y<2 || y>5)
            {
                score ++;
                if (y<1 || y>6)
                    score++;
            }

            score += 15;
            break;
        
        case LIGHT_KING:
            score += 25;  // +25pkt za każdego króla
            if (y<2 || y>5)
            {
                score ++;
                if (y<1 || y>6)
                    score++;
            }

            score += 15;
            break;
        
        case DARK_KING:
            score += 25;
            
            if (y<2 || y>5)
            {
                score++;
                if (y<1 || y>6)
                    score ++;
            }

            score += 15;
            break;
    }
    return score;
}

// zwróć wartość w zależności od stanu gry
int Board::evaluateBoard(bool color, bool side) 
{
    switch (checkState(color)) 
    {
        case -1:
            return 0;  // 0pkt za remis
       
        case 0:
            break;
       
        case 1:
            return side ? -10000 : 10000;   // -10000pkt za przegraną, +10000pkt za wygraną
       
        case 2:
            return side ? 10000 : -10000;
    }

    int dark{}, light{};

    for (int i=0; i<8; i++) 
    {
        for (int j=0; j<8; j++) 
        {
            if (board[i][j] == LIGHT_KING || board[i][j] == LIGHT_PAWN)
                light += evaluateField(i, j);
         
            else if (board[i][j] != EMPTY) 
                dark += evaluateField(i, j);
        }
    }
    
    if (side)
        return dark - light;
    
    else 
        return light - dark;
}
