#include <iostream>
#include <vector>
#include <map>

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
            auto colours = getMap(tile.getColour(), row, col);
            auto shapes = getMap(tile.getShape(), row, col);
            int shapesColours[] = { colours["topRight"] + shapes["topRight"],
                                    colours["topLeft"] + shapes["topLeft"],
                                    colours["bottomLeft"] + shapes["bottomLeft"],
                                    colours["bottomRight"] + shapes["bottomRight"] };

            bool offDiagonal = colours["topRight"] == colours["bottomLeft"] || shapes["topRight"] == shapes["bottomLeft"];
            bool mainDiagonal = colours["topLeft"] == colours["bottomRight"] || shapes["topLeft"] == shapes["bottomRight"];

            int positions = 0;

            for (int i = 0; i < (int) shapes.size(); ++i) {
                if (shapesColours[i] != 0) {
                    positions++;
                }
            }

            if (positions == 1) {
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
    int inputColourShape = 0;
    bool odd = row % 2 == 1;
    bool border = (edgeRow(row, rowDirection) || edgeCol(col, odd, right));
    col = calculateCol(odd, right, col);
    row = row + rowDirection;
    odd = !odd;

    while (!border && grid[row][col] != nullptr) {
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

        border = edgeRow(row, rowDirection) || edgeCol(col, odd, right);
        col = calculateCol(odd, right, col);
        row = row + rowDirection;
        odd = !odd;
    }

    if (inputColourShape != 0) {
        inputColourShape = colourShape;
    }

    return inputColourShape;
}

int Board::calculateCol(bool odd, bool right, int col) {
    if (odd) {
        if (right) {
            col++;
        }
    } else {
        if (!right) {
            col--;
        }
    }
    return col;
}

bool Board::edgeRow(int row, int rowDirection) {
    return (row == 0 && rowDirection == -1) || (row == ROWS - 1 && rowDirection == 1);
}

bool Board::edgeCol(int col, bool odd, bool right) {
    return (col == 0 && !odd && !right) || (col == COLS - 1 && odd && right);
}

std::map<std::string, int> Board::getMap(int shapeColour, int row, int col) {
    std::map<std::string, int> coloursShapes;
    coloursShapes["topRight"] = validateRow(shapeColour, row, col, 1, true);
    coloursShapes["bottomRight"] = validateRow(shapeColour, row, col, -1, true);
    coloursShapes["bottomLeft"] = validateRow(shapeColour, row, col, -1, false);
    coloursShapes["topLeft"] = validateRow(shapeColour, row, col, 1, false);
    return coloursShapes;
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


