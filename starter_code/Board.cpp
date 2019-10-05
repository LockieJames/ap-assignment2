#include <iostream>
#include <vector>
#include <map>

#include "Board.h"
#include "Tile.h"

Board::Board() {
    grid = std::vector<std::vector<Tile*>> (ROWS);
    for (int i = 0; i < ROWS; ++i) {
        grid.at(i) = std::vector<Tile*> (COLS);
    }
}

Board::Board(std::vector<std::vector<Tile*>> loadedGrid){
    this->grid = loadedGrid;
}

Board::~Board() {
    std::cout << "Got to start delete board" << std::endl;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            delete grid[i][j];
        }
    }
    std::cout << "Got to end delete board" << std::endl;
}

// Returns number of points earned in this turn
int Board::placeTile(Tile &tile, char rowInput, int col) {
    // TODO: scoring system
    scoreTurn = 0;
    int row = rowInput - A_VALUE;

    bool validInput = false;
    if ((row + col) % 2 == 0)
        validInput = true;

    col = col / 2;

    if (grid[row][col] != nullptr)
        validInput = false;

    if (row <= ROWS && col <= COLS && validInput) {
        emptyBoard = isEmpty();
        if (emptyBoard) {
            std::cout << "Got to empty board" << std::endl;
            
            grid[row][col] = &tile;
            emptyBoard = isEmpty();
            scoreTurn = 1;
        } else {
            std::cout << "Got to else board" << std::endl;
            
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
                scoreTurn += ONE_ROW;
            } else if (positions == 2) {
                if((shapesColours[0] != 0 && shapesColours[2] != 0)
                   || (shapesColours[1] != 0 && shapesColours[3] !=0)) {
                    if (offDiagonal || mainDiagonal) {
                        grid[row][col] = &tile;
                        scoreTurn += ONE_ROW;
                    }
                } else {
                    grid[row][col] = &tile;
                    scoreTurn += TWO_ROWS;
                }
            } else if (positions == 3) {
                if (offDiagonal || mainDiagonal) {
                    grid[row][col] = &tile;
                    scoreTurn += TWO_ROWS;
                }
            } else if (positions == 4){
                if (offDiagonal && mainDiagonal) {
                    grid[row][col] = &tile;
                    scoreTurn += TWO_ROWS;
                }
            }
        }
    }
    return scoreTurn;
}

int Board::validateRow(int colourShape, int row, int col, int rowDirection, bool right) {
    int inputColourShape = 0;
    bool odd = row % 2 == 1;
    bool border = (edgeRow(row, rowDirection) || edgeCol(col, odd, right));
    col = calculateCol(odd, right, col);
    row = row + rowDirection;
    odd = !odd;
    int numberOfCheckedTiles = 0;

    while (!border && grid[row][col] != nullptr && numberOfCheckedTiles <= QWIRKLE) {
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
            numberOfCheckedTiles++;
        }

        border = edgeRow(row, rowDirection) || edgeCol(col, odd, right);
        col = calculateCol(odd, right, col);
        row = row + rowDirection;
        odd = !odd;
    }

    if (inputColourShape != 0) {
        inputColourShape = colourShape;
    }

    if (numberOfCheckedTiles == QWIRKLE) {
        std::cout << "QWIRKLE!!!";
    }
    scoreTurn += numberOfCheckedTiles;

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

bool Board::printBoard(std::ostream &destination) {
    char letter = 'A';
    printCoord(destination, 0);
    printBorder(destination);
    bool odd = false;

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < (COLS + 1); ++j) {
            if (j == 0) {
                destination << letter << " ";
                if (odd) {
                    destination << SPACE;
                }
                letter++;
            } else {
                destination << PATTERN;
                if (grid[i][j - 1]) {
                    destination << grid[i][j - 1]->getColour() << grid[i][j - 1]->getShape();
                } else {
                    destination << "  ";
                }
            }
        }
        destination << "  |";
        odd = !odd;
        destination << std::endl;
    }

    printBorder(destination);
    printCoord(destination, 1);
    return true;
}

void Board::printBorder(std::ostream &destination) {
    destination << SPACE;
    for (int k = 0; k < COLS + 1; ++k) {
        destination << BORDER_PATTERN;
    }
    destination << std::endl;
}

void Board::printCoord(std::ostream &destination, int startNumber) {
    destination << " " << SPACE;
    if(startNumber % 2 == 1) {
        destination << SPACE;
    }
    for (int i = 0; i < COLS; ++i) {
        destination << "  " << startNumber << "  ";
        startNumber += 2;
    }
    destination << std::endl;
}
