#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "Board.hpp"

#ifndef _GUI_HPP
#define _GUI_HPP

#define WINDOW_NAME "Othello player"
#define WINDOW_SCALE 0.5

#define DIVIDER_THICKNESS 10
#define DIVIDER_POSITION 0.6

#define BOARD_OFFSET 30
#define BOARD_THICKNESS 4

#define DISC_RADIUS_RATIO 0.4

typedef struct Base_
{
    sf::RectangleShape background;
    std::vector<sf::RectangleShape> grid;
    int offset, step;
} Base;

class BoardGui {
    int edgeSize, nCells;
    Base base;
    std::vector<sf::CircleShape> discs;

public:
    BoardGui() = delete;
    BoardGui(int edgeSize);
    ~BoardGui() {}
    void update(std::vector<char> &cells);
    void place(int height);
    void click(int x, int y, OthelloBoard &board);
    void draw(sf::RenderWindow &window);
};

class Game {
    OthelloBoard &board;
    sf::RenderWindow window;
    sf::RectangleShape divider;
    BoardGui boardGui;

public:
    Game() = delete;
    Game(OthelloBoard &board, const std::string windowName=WINDOW_NAME);
    ~Game() {};
    void play();
    void closeWindow();
};

// class Score
// {
//     sf::Vector2i position;
//     sf::Vector2i size;
//     int score;
// public:
//     Score();
//     ~Score();
// };

#endif
