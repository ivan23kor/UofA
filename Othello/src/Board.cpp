#include <iomanip>
#include <iostream>
#include "Board.hpp"

using namespace std;

const int INF = std::numeric_limits<int>::max();
const int nINF = std::numeric_limits<int>::min(); // negative infinity


void printVector(const std::vector<int> &v) {
    if (v.empty()) {
        std::cout << "<empty vector>" << std::endl;
        return;
    }
    for (const auto &it: v) {
        std::cout << it << " ";
    }
    std::cout << std::endl;
}

void printMovesMap(const MovesMap &moves) {
    if (moves.empty()) {
        std::cout << "<empty map>" << std::endl;
        return;
    }
    for (const auto &[moves, flips]: moves) {
        std::cout << "[" << moves << "]: ";
        printVector(flips);
    }
}

SquareBoard::SquareBoard(int edgeSize) {
    this->edgeSize = edgeSize;
    nCells = edgeSize * edgeSize;
    cells = vector<char>(nCells, EMPTY);
}

bool SquareBoard::print() const {
    // Print color
    // cout << "\033[2;40m" << " " << "\033[0m";
    if (player == BLACK) {
        cout << "\033[;40m\033[1;37m" << "B " << "\033[0m\033[0m";
    } else {
        cout << "\033[;47m\033[1;30m" << "W " << "\033[0m\033[0m";
    }

    for (size_t i = 0; i < cells.size(); ++i)
    {
        // Prepend TAB
        if (i % edgeSize == 0) {
            cout << '\t';
        }

        // Print cell
        cout << cells[i];
        // if (cells[i] == BLACK) {
        //     cout << "\033[;40m" << "bb" << "\033[0m";
        // } else if (cells[i] == WHITE) {
        //     cout << "\033[;47m\033[1;30m" << "ww" << "\033[0m\033[0m";
        // } else {
        //     cout << "\033[;42m" << "  " << "\033[0m";;
        // }

        // Print the index board besides
        if ((i + 1) % edgeSize == 0) {
            cout << "\t\t";
            for (int j = i - edgeSize + 1; j <= i; ++j){
                cout << setfill(' ') << setw(3) << j;
            }
            cout << endl;
        }
    }
    return true;
}

bool SquareBoard::put(int pos, char cell) {
    if (!validPosition(pos)) {
        return false;
    }
    cells[pos] = cell;
    return true;
}

OthelloBoard::OthelloBoard(int edgeSize, char player)
                           : SquareBoard::SquareBoard(edgeSize) {
    if (edgeSize < MINIMUM_OTHELLO_BOARD_SIZE) {
        cerr << "Minimum Othello board size is " << MINIMUM_OTHELLO_BOARD_SIZE
             << endl;
        exit(1);
    }

    setPlayer(player);

    blackCount = 2;
    whiteCount = 2;

    int start = (edgeSize + 1) * (edgeSize / 2 - 1);
    put(start, WHITE);
    put(start + 1, BLACK);
    put(start + edgeSize, BLACK);
    put(start + edgeSize + 1, WHITE);
}

void OthelloBoard::printMoves() const {
    std::cout << "Possible moves:" << std::endl;
    printMovesMap(moves);
}

bool OthelloBoard::isGameOver() {
    if (moves.size() != 0) {
        return false;
    }

    exploreMoves();
    int curPlayerNMoves = moves.size();

    changePlayer(); // change to opponent
    exploreMoves();
    int opponentNMoves = moves.size();

    changePlayer(); // change back to the current player
    return curPlayerNMoves == 0 && opponentNMoves == 0;
}

void OthelloBoard::exploreDirection(int cellPos, int inc) {
    vector<int> flips; // positions of the discs to flip
    char opponent = player == BLACK ? WHITE : BLACK;
    int pos = cellPos + inc;
    for (; ((inc < 0 &&  /* upper-left */
             pos >= 0 && /* upper border */
             pos % edgeSize != 0) || /* left border */
            (inc > 0 && /* bottom-right */
             pos < nCells && /* bottom border */
             (pos + 1) % edgeSize != 0)) && /* right border */
            cells[pos] == opponent; /* still on line */
            pos += inc) {
        flips.push_back(pos);
    }
    // advanced more than once and found an empty cell
    if (pos - cellPos != inc && cells[pos] == EMPTY) {
        moves[pos].insert(moves[pos].end(), flips.begin(), flips.end());
    }
}

void OthelloBoard::exploreMoves() {
    static size_t cellPos = 0; // index for player cell iteration
    moves.clear();
    for (cellPos = 0; cellPos < nCells; ++cellPos){
        if (cells[cellPos] != player) {
            continue;
        }
        // Look left
        exploreDirection(cellPos, -1);
        // Look NW
        exploreDirection(cellPos, -edgeSize - 1);
        // Look up
        exploreDirection(cellPos, -edgeSize);
        // Look NE
        exploreDirection(cellPos, -edgeSize + 1);
        // Look right
        exploreDirection(cellPos, 1);
        // Look SE
        exploreDirection(cellPos, edgeSize + 1);
        // Look down
        exploreDirection(cellPos, edgeSize);
        // Look SW
        exploreDirection(cellPos, edgeSize - 1);
    }
}

void OthelloBoard::move(int to) {
    if (to == PASSING_MOVE) {
        changePlayer();
        return;
    }

    cells[to] = player;
    for (const auto &flip: moves[to]) {
        cells[flip] = player;
    }

    int nFlips = moves[to].size();
    if (player == BLACK) {
        blackCount += nFlips + 1;
        whiteCount -= nFlips;
    } else {
        blackCount -= nFlips;
        whiteCount += nFlips + 1;
    }

    changePlayer();
}

int OthelloBoard::random() {
    return moves.begin()->first;
}

int OthelloBoard::greedy() {
    int bestMove = PASSING_MOVE, bestLen = 0;
    for (auto &[to, flips]: moves) {
        if (flips.size() > bestLen) {
            bestLen = flips.size();
            bestMove = to;
        }
    }
    return bestMove;
}

// TODO
int OthelloBoard::minimax() {
    // int max = nINF, min = INF;

    // int i,j,value = 1;

    // if (isGameOver()) {
    //     return score();
    // }

    // vector<int> scores(moves.size(), 0);
    // for (auto &[move, flips]: moves) {
    //     cout << "Making move " << move << endl;
    //     this->move(move);
    // }

    // return 0;
}
