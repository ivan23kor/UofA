#include <iostream>
#include "Board.hpp"

#ifndef __AGENT_HPP
#define __AGENT_HPP

using namespace std;

class Agent
{
    OthelloBoard &board;
    bool AI; // true for AI, false for human
public:
    Agent() = delete;
    Agent(bool AI, OthelloBoard &board) : AI(AI), board(board) {}; // true for AI, false for human
    ~Agent() {}
    int getMove() {
        int move = PASSING_MOVE;
        board.printMoves();
        if (AI) {
            move = board.greedy();
            cout << "I move to " << move << endl;
        } else {
            move = readHumanMove(board.getMoves());
        }
        return move;
    }
    int readHumanMove(const MovesMap &moves) {
        int move = PASSING_MOVE;

        do {
            cout << "Which one are you choosing? ";
            cin >> move;
        } while (moves.find(move) == moves.end() && move != PASSING_MOVE);
        return move;
    }

};

#endif
