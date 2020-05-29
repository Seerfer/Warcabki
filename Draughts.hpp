#ifndef DRAUGHTS_HPP
#define DRAUGHTS_HPP

#include <unistd.h>
#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "AI.hpp"

const int size = 850;  // rozmiar planszy w pikselach

class Draughts
{
    bool player;  // człowiek vs komputer
    bool side;    // jasne vs ciemne

    sf::RenderWindow win;
    sf::Texture textures[6];
    sf::Sprite sprites[6];

    Board board;

public:

    Draughts(); // ustawienie tekstur i strony rozpoczynającej grę
    void play();  // zapętlnienie rund gry
    void playerMove(); // ruch gracza
    void computerMove(bool second = false); // ruch komputera
    bool gameStart(); // rozpoczęcie sekwencji gry
    int gameOver(int state); // zakończenie sekwencji gry
    int mouse(); // obsługa kursora
    bool handleEvents();  // obsługa wydarzeń w oknie gry
    void draw();  // ustawienie pionów na planszy
};

#endif
