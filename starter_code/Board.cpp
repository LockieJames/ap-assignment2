#include <iostream>
#include "Board.h"
#include "TileCodes.h"
#include <vector>

Board::Board() {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            grid[j][i] = nullptr;
        }
    }
}

Board::~Board() {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (grid[j][i] != nullptr) {
                delete grid[i][j];
                grid[j][i] = nullptr;
            }
        }
    }
}

bool Board::placeTile(Tile &tile, char rowInput, int col) {
    bool validPlace = false;
    int row = rowInput - A_VALUE;
    col = col / 2;

    if (row <= ROWS && col <= COLS) {
        std::vector<Tile*> surroundings;
        bool odd = false;
        if (row % 2 == 1) {
            odd = true;
        }
        int length;

        if (row == 0 && col == COLS - 1) {
            surroundings.push_back(grid[col][row - 1]);
            length = 1;
        } else if (row == ROWS - 1 && col == COLS - 1) {
            surroundings.push_back(grid[col][row + 1]);
            length = 1;
        } else if (row == 0) {
            surroundings.push_back(grid[col][row + 1]);
            surroundings.push_back(grid[col - 1][row + 1]);
            length = 2;
        } else if (row == ROWS - 1) {
            surroundings.push_back(grid[col][row - 1]);
            surroundings.push_back(grid[col + 1][row - 1]);
            length = 2;
        } else {
            if (odd) {
                surroundings.push_back(grid[col + 1][row - 1]);
                surroundings.push_back(grid[col + 1][row + 1]);
            } else {
                surroundings.push_back(grid[col - 1][row - 1]);
                surroundings.push_back(grid[col - 1][row + 1]);
            }

            surroundings.push_back(grid[col][row - 1]);
            surroundings.push_back(grid[col][row + 1]);
            length = 4;
        }

        int emptySurroundings[length];

        int filledSpaces = 0;
        for (int i = 0; i < surroundings.size(); ++i) {
            emptySurroundings[i] = (surroundings.at(i) == nullptr);
            if (!emptySurroundings[i]) {
                filledSpaces++;
            }
        }

        if (filledSpaces < 3 && filledSpaces != 0 && !(filledSpaces == 2 && col == 0)) {
            if (filledSpaces == 2) {
                if (surroundings.size() == 2) {
                    validPlace = placeTwo(tile, *surroundings.at(0), *surroundings.at(1), row, col);
                } else {
                    if ((emptySurroundings[0] || emptySurroundings[3]) && (emptySurroundings[2] || emptySurroundings[1])) {
                        if (!(emptySurroundings[0] && emptySurroundings[2])) {
                            validPlace = placeTwo(tile, *surroundings.at(0), *surroundings.at(2), row, col);
                        } else {
                            validPlace = placeTwo(tile, *surroundings.at(1), *surroundings.at(3), row, col);
                        }
                    }
                }
            } else {
                for (int i = 0; i < surroundings.size(); ++i) {
                    if (!emptySurroundings[i]){
                        if (validateField(tile,*surroundings.at(i))) {
                            grid[col][row] = &tile;
                            validPlace = true;
                        }
                    }
                }
            }
        }
    }
    return validPlace;
}

bool Board::placeTwo(Tile &currentTile, Tile &check1Tile, Tile &check2Tile, int row, int col) {
    bool valid = false;
    if (validateField(check1Tile, currentTile) && validateField(check2Tile, currentTile)){
        grid[col][row] = &currentTile;
        valid = true;
    }
    return valid;
}

bool Board::validateField(Tile& tile, Tile& otherTile) {
    return equalShape(otherTile, tile) || equalColour(otherTile, tile);
}

bool Board::equalColour(Tile& tile, Tile& otherTile) {
    return  otherTile.getColour() == tile.getColour();
}

bool Board::equalShape(Tile& tile, Tile& otherTile) {
    return  otherTile.getShape() == tile.getShape();
}

bool Board::printBoard() {
    char letter = 'A';
    printCoord(0);
    printBorder();
    bool odd = false;

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < (COLS + 1); ++j) {
            if (j == 0) {
                std::cout << letter << " ";
                if (odd) {
                    std::cout << SPACE;
                }
                letter++;
            } else {
                std::cout << PATTERN;
                if (grid[j - 1][i] != nullptr) {
                    std::cout << grid[j - 1][i]->getColour() << grid[j - 1][i]->getShape();
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


