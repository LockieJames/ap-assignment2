#include <iostream>
#include <vector>

#include "Board.h"
#include "Tile.h"

Board::Board() {
    grid = std::vector<std::vector<Tile* > > (ROWS);
    for (int i = 0; i < ROWS; ++i) {
        grid.at(i) = std::vector<Tile*> (COLS);
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

bool Board::placeTile(Tile &tile, char rowInput, int col) {
    bool validPlace = false;
    int row = rowInput - A_VALUE;

    bool validInput = false;
    if ((row + col) % 2 == 0)
        validInput = true;

    col = col / 2;

    if (grid[row][col] != nullptr)
        validInput = false;


    if (row <= ROWS && col <= COLS && validInput) {
        if (emptyBoard) {
            emptyBoard = isEmpty();
            if (emptyBoard) {
                grid[row][col] = &tile;
                validPlace = true;
                emptyBoard = isEmpty();
            }
        } else {
            // TODO put in array / map nicely
            int topRightColour = validateRow(tile.getColour(), row, col, 1, true);
            int topLeftColour = validateRow(tile.getColour(), row, col, 1, false);
            int bottomLeftColour = validateRow(tile.getColour(), row, col, -1, false);
            int bottomRightColour = validateRow(tile.getColour(), row, col, -1, true);

            int colours[] = { topRightColour, topLeftColour, bottomLeftColour, bottomRightColour };

            int topRightShape = validateRow(tile.getShape(), row, col, 1, true);
            int topLeftShape = validateRow(tile.getShape(), row, col, 1, false);
            int bottomLeftShape = validateRow(tile.getShape(), row, col, -1, false);
            int bottomRightShape = validateRow(tile.getShape(), row, col, -1, true);

            int shapes[] = { topRightShape, topLeftShape, bottomLeftShape, bottomRightShape };

            int shapesColours[] = { topRightShape + topRightColour, topLeftShape + topLeftColour,
                                    bottomLeftShape + bottomLeftColour, bottomRightShape + bottomRightColour};
            bool offDiagonal = topRightColour == bottomLeftColour || topRightShape == bottomRightShape;
            bool mainDiagonal = topLeftColour == bottomRightColour || topLeftShape == bottomRightShape;

            int positions = 0;
            for (int j = 0; j < 4; ++j) {
                if ((colours[j] || shapes[j]) != 0) {
                    positions++;
                }
            }

            if (positions == 1 ) {
                grid[row][col] = &tile;
                validPlace = true;
            } else if (positions == 2) {
                if((shapesColours[0] != 0 && shapesColours[2] != 0)
                   || (shapesColours[1] != 0 && shapesColours[3] !=0)) {
                    if (offDiagonal || mainDiagonal) {
                        grid[row][col] = &tile;
                        validPlace = true;
                    }
                } else {
                    grid[row][col] = &tile;
                    validPlace = true;
                }
            } else if (positions == 3) {
                if (offDiagonal || mainDiagonal) {
                    grid[row][col] = &tile;
                    validPlace = true;
                }
            } else if (positions == 4){
                if (offDiagonal && mainDiagonal) {
                    grid[row][col] = &tile;
                    validPlace = true;
                }
            }
        }
    }
    return validPlace;
}

int Board::validateRow(int colourShape, int row, int col, int rowDirection, bool right) {
    // TODO reduce code repetition
    int inputColourShape = 0;
    bool odd = row % 2 == 1;
    bool notBorder = true;
    if ((row == 0 && rowDirection == -1) || (row == ROWS - 1 && rowDirection == 1))
        notBorder = false;

    if ((col == 0 && !odd && !right) || (col == COLS - 1 && odd && right))
        notBorder = false;

    if (odd) {
        if (right)
            col++;
    } else {
        if (!right)
            col--;
    }
    row = row + rowDirection;
    odd = !odd;

    while (notBorder && grid[row][col] != nullptr) {
        if ((row == 0 && rowDirection == -1) || (row == ROWS - 1 && rowDirection == 1))
            notBorder = false;

        if ((col == 0 && !odd && !right) || (col == COLS - 1 && odd && right))
            notBorder = false;

        int positionValue = 0;

        if (colourShape < 6) {
            positionValue = grid[row][col]->getShape();
        } else {
            positionValue = grid[row][col]->getColour();
        }

        if (colourShape != positionValue){
            inputColourShape = 0;
        } else {
            inputColourShape = colourShape;
        }

        if (odd) {
            if (right) {
                col++;
            }
        } else {
            if (!right) {
                col--;
            }
        }
        row = row + rowDirection;
        odd = !odd;
    }
    if (inputColourShape != 0) {
        inputColourShape = colourShape;
    }

    return inputColourShape;
}

bool Board::isEmpty() {
    bool isEmpty = true;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (grid[i][j] != nullptr) {
                isEmpty = false;
            }
        }
    }
    return isEmpty;
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
                if (grid[i][j - 1] != nullptr) {
                    std::cout << grid[i][j - 1]->getColour() << grid[i][j - 1]->getShape();
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


