#include <iostream>
#include "Gui.hpp"

using namespace sf;
using namespace std;


Game::Game(OthelloBoard &board, const std::string windowName)
        : board(board), boardGui(board.getEdgeSize()) {
    // Center window
    VideoMode mode = VideoMode::getDesktopMode();
    int width = mode.width * WINDOW_SCALE, height = mode.height * WINDOW_SCALE;
    window.create(VideoMode(width, height), windowName);
    window.setPosition(Vector2i(mode.width * (1 - WINDOW_SCALE) / 2,
                                mode.height * (1 - WINDOW_SCALE) / 2));

    // BoardGui and score dividing line
    divider = RectangleShape(Vector2f(DIVIDER_THICKNESS, height));
    divider.setFillColor(Color::Black);
    divider.setPosition(height, 0);

    // Place board in the center of the left part
    boardGui.place(height);
    boardGui.update(board.getCells());
}

void Game::play() {
    Event event;
    board.exploreMoves();

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    boardGui.click(event.mouseButton.x, event.mouseButton.y,
                        board);
                }
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Space) {
                    board.move(PASSING_MOVE);
                }
            }
            if      (event.type == Event::MouseButtonPressed ||
                     event.key.code == Keyboard::Space) {
                if (board.isGameOver()) {
                    closeWindow();
                }
                board.exploreMoves();
            }
            if (event.type == Event::Closed) {
                closeWindow();
            }
        }

        window.clear(Color::White);
        boardGui.draw(window);
        window.draw(divider);
        window.display();
    }
}

void Game::closeWindow() {
    cout << "Closing the window." << endl;
    window.close();
    return;
}

BoardGui::BoardGui(int edgeSize) {
    this->edgeSize = edgeSize;
    nCells = edgeSize * edgeSize;

    base.offset = BOARD_OFFSET;

    base.background.setFillColor(Color(0, 160, 12));
    base.background.setOutlineColor(Color::Black);
    base.background.setOutlineThickness(BOARD_THICKNESS);

    base.grid = vector<RectangleShape> (2 * (edgeSize - 1),
                                        RectangleShape(Vector2f(0, 0)));
    for (auto &line: base.grid) {
        line.setFillColor(Color::Black);
    }

    discs = vector<CircleShape> (nCells, CircleShape(0));
    // Set discs to be invisible in the beginning ...
    for (int i = 0, size = discs.size(); i < size; ++i) {
        discs[i].setFillColor(Color::Transparent);
    }
    // ... except for the initial
    int start = (edgeSize + 1) * (edgeSize / 2 - 1);
    discs[start].setFillColor(Color::White);
    discs[start + 1].setFillColor(Color::Black);
    discs[start + edgeSize].setFillColor(Color::White);
    discs[start + edgeSize + 1].setFillColor(Color::Black);
}

void BoardGui::update(vector<char> &cells) {
    for (int i = 0, e = cells.size(); i < e; ++i) {
        if (cells[i] == BLACK) {
            discs[i].setFillColor(Color::Black);
        } else if (cells[i] == WHITE) {
            discs[i].setFillColor(Color::White);
        } else {
            discs[i].setFillColor(Color::Transparent);
        }
    }
}

void BoardGui::place(int height) {
    // For grid lines
    int size = height - 2 * base.offset;
    base.step = size / (float)edgeSize; // presume the board is always square
    base.background.setPosition(base.offset, base.offset);
    base.background.setSize(Vector2f(size, size));

    for (int i = 0; i < edgeSize - 1; ++i) {
        // Vertical
        base.grid[i].setSize(Vector2f(BOARD_THICKNESS, size));
        base.grid[i].setPosition(base.offset + (i + 1) * base.step, base.offset);
        // Horizontal
        base.grid[i + edgeSize - 1].setSize(Vector2f(size, BOARD_THICKNESS));
        base.grid[i + edgeSize - 1].setPosition(base.offset,
            base.offset + (i + 1) * base.step);
        base.grid[i + edgeSize - 1].setRotation(0);
    }

    float radius = DISC_RADIUS_RATIO * base.step;
    float shift = (base.step - 2 * radius) / 2;
    for (auto &disc: discs) {
        disc.setRadius(radius);
    }
    for (int i = 0, size = discs.size(); i < size; ++i) {
        discs[i].setPosition(base.offset + (i % edgeSize) * base.step + shift,
                             base.offset + (i / edgeSize) * base.step + shift);
    }
}
void BoardGui::click(int x, int y, OthelloBoard &board) {
    int cellNumber = (y - base.offset) / base.step * edgeSize +
                     (x - base.offset) / base.step;
    // board.print();
    if (board.getMoves().find(cellNumber) != board.getMoves().end()) {
        board.move(cellNumber);
        update(board.getCells());
    }
}

void BoardGui::draw(RenderWindow &window) {
    window.draw(base.background);
    for (int i = 0; i < 2 * (edgeSize - 1); ++i) {
        window.draw(base.grid[i]);
    }
    for (auto &disc: discs) {
        window.draw(disc);
    }
}
