#include <iostream>
#include "Board.h"
#include "TileCodes.h"

Board::Board() {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            grid[i][j] = nullptr;
        }
    }
}

Board::~Board() {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (grid[i][j] != nullptr) {
                delete grid[i][j];
                grid[i][j] = nullptr;
            }
        }
    }
}

bool Board::printBoard() {
    char letter = 'A';
    printCoord(0);
    printBorder();
    bool odd = false;

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < (COLS) + 1; ++j) {
            if (j == 0) {
                std::cout << letter << " ";
                if (odd) {
                    std::cout << SPACE;
                }
                letter++;
            } else {
                std::cout << PATTERN;
                if (grid[j][i] != nullptr) {
                    std::cout << grid[i][j]->getColour() << grid[i][j]->getShape();
                } else {
                    std::cout << "  ";
                }
            }
        }
        std::cout << "  |";
        odd = !odd;
        std::cout << std::endl;
    }

    printBorder();
    printCoord(1);
    return true;
}

void Board::printBorder() {
    std::cout << SPACE;
    for (int k = 0; k < COLS + 1; ++k) {
        std::cout << BORDER_PATTERN;
    }
    std::cout << std::endl;
}

void Board::printCoord(int startNumber) {
    std::cout << " " << SPACE;
    if(startNumber % 2 == 1) {
        std::cout << SPACE;
    }
    for (int i = 0; i < COLS; ++i) {
        std::cout << "  " << startNumber << "  ";
        startNumber += 2;
    }
    std::cout << std::endl;
}


