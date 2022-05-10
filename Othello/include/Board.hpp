#include <map>
#include <string>
#include <vector>

#ifndef _BOARD_HPP
#define _BOARD_HPP

#define MINIMUM_OTHELLO_BOARD_SIZE 3
#define PASSING_MOVE -1

typedef std::map<int, std::vector<int>> MovesMap;

enum Cell: char {EMPTY = '.', BLACK = 'x', WHITE = 'o'};

class SquareBoard
{
protected:
    int edgeSize, nCells;
    char player;
    std::vector<char> cells;

    bool put(int pos, char cell);

public:
    // Setup
    SquareBoard(int edgeSize);
    ~SquareBoard() {};

    bool print() const;

    // Setters getters
    int getEdgeSize() {return edgeSize;}

    // Move handling
    bool validPosition(int pos) {return 0 <= pos && pos < nCells;}
};

class OthelloBoard : public SquareBoard
{
    int blackCount, whiteCount;
    MovesMap moves;
public:
    // Setup
    OthelloBoard(int edgeSize, char player=BLACK);

    void printMoves() const;

    // Setters getters
    void setPlayer(char player) {this->player = player;}
    char getPlayer() {return player;}
    MovesMap &getMoves() {return moves;}
    std::vector<char> &getCells() {return cells;}
    std::vector<char> copyCells() {return std::vector<char> (cells);}

    // Move handling
    bool isGameOver();
    void changePlayer() {setPlayer(player == BLACK ? WHITE : BLACK);}
    int score() const {return blackCount - whiteCount;}
    void exploreDirection(int cellPos, int inc);
    void exploreMoves();
    void move(int to);

    // Algorithms
    int random();
    int greedy();
    int minimax();
};

void printVector(const std::vector<int> &v);
void printMovesMap(const MovesMap &moves);
#endif
