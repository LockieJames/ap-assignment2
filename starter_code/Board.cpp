#include <iostream>
#include <vector>
#include <map>

#include "Board.h"
#include "Menu.h"

Board::Board() {
    grid = std::vector<std::vector<Tile*>> (ROWS);
    for (int i = 0; i < ROWS; ++i) {
        grid.at(i) = std::vector<Tile*> (COLS);
    }
    firstRowOffset = false;
}

Board::Board(std::vector<std::vector<Tile*>> loadedGrid, bool firstRowOffset){
    this->grid = loadedGrid;
    this->firstRowOffset = firstRowOffset;
}

Board::~Board() {
//    std::cout << "Got to start delete board" << std::endl;
    for (int i = 0; i < (int) grid.size(); ++i) {
        for (int j = 0; j < (int) grid.at(i).size(); ++j) {
            delete grid[i][j];
        }
    }
//    std::cout << "Got to end delete board" << std::endl;
}

// Returns number of points earned in this turn
int Board::placeTile(Tile &tile, char rowInput, int col) {
    // TODO: scoring system
    scoreTurn = 0;
    int row = rowInput - A_VALUE;

    bool validInput = false;
    if (!firstRowOffset){
        if ((row + col) % 2 == 0) {
            validInput = true;
        }
    } else {
        if ((row + col + 1) % 2 == 0) {
            validInput = true;
        }
    }

    col = col / 2;

    if (grid[row][col] != nullptr) {
        validInput = false;
    }


    if (row <= (int) grid.size() - 1 && col <= (int) grid.at(0).size() - 1 && validInput) {
        emptyBoard = isEmpty();
        if (emptyBoard) {
            grid[row][col] = &tile;
            emptyBoard = isEmpty();
            scoreTurn = 1;
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

    if (scoreTurn != 0){
        expandBoard();
    }

    return scoreTurn;
}

int Board::validateRow(int colourShape, int row, int col, int rowDirection, bool right) {
    int inputColourShape = 0;
    bool odd;
    if (!firstRowOffset){
        odd = row % 2 == 1;
    } else {
        odd = (row + 1) % 2 == 1;
    }

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

    std::cout << "Number of checked Tiles: " << std::endl;
    
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
    return (row == 0 && rowDirection == -1) || (row == (int) grid.size() - 1 && rowDirection == 1);
}

bool Board::edgeCol(int col, bool odd, bool right) {
    return (col == 0 && !odd && !right) || (col == (int) grid.at(0).size() - 1 && odd && right);
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
    for (int i = 0; i < (int) grid.size(); ++i) {
        for (int j = 0; j < (int) grid.at(i).size(); ++j) {
            if (grid[i][j] != nullptr) {
                isEmpty = false;
            }
        }
    }
    return isEmpty;
}

bool Board::printBoard(std::ostream &destination) {
    Menu menu;
    char letter = 'A';
    printCoord(destination, 0);
    printBorder(destination);
    bool odd = firstRowOffset;

    for (int i = 0; i < (int) grid.size(); ++i) {
        for (int j = 0; j < ((int) grid.at(i).size() + 1); ++j) {
            if (j == 0) {
                destination << letter << " ";
                if (odd) {
                    destination << SPACE;
                }
                letter++;
            } else {
                destination << PATTERN;
                if (grid[i][j - 1]) {
                    destination << menu.printColour(grid[i][j - 1]->getColour(), destination)
                    << menu.printShape(grid[i][j - 1]->getShape(), destination);
                } else {
                    destination << "  ";
                }
            }
        }
        destination << " |";
        odd = !odd;
        destination << std::endl;
    }

    printBorder(destination);
    printCoord(destination, 1);
    return true;
}

void Board::printBorder(std::ostream &destination) {
    destination << SPACE;
    for (int k = 0; k < (int) grid.at(0).size(); ++k) {
        destination << BORDER_PATTERN;
    }
    destination << BORDER_PATTERN_END;
    destination << std::endl;
}

void Board::printCoord(std::ostream &destination, int startNumber) {
    destination << " " << SPACE;
    if(startNumber % 2 == 1) {
        destination << SPACE;
    }
    for (int i = 0; i < (int) grid.at(0).size(); ++i) {
        if (std::to_string(startNumber).length() == 2){
            destination << " " << startNumber << "  ";
        } else {
            destination << "  " << startNumber << "  ";
        }
        startNumber += 2;
    }
    destination << std::endl;
}

void Board::expandBoard(){

    printBoard(std::cout);

//    std::cout << "checking front of rows needs to be expanded" << std::endl;
    for (auto i : grid.at(0)){
        if (i && (int) grid.size() <= MAX_ROWS){
            grid.insert(grid.begin(), std::vector<Tile *>(grid.at(grid.size() - 1).size()));
//            std::cout << "expanded front of rows" << std::endl;
            firstRowOffset = !firstRowOffset;
        }
    }

//    std::cout << "checking back of rows needs to be expanded" << std::endl;
    // expand back row
    for (auto i : grid.at(grid.size() - 1)){
        if (i && (int) grid.size() <= MAX_ROWS){
            grid.insert(grid.end(), std::vector<Tile *>(grid.at(0).size()));
//            std::cout << "expanded back of rows" << std::endl;
        }
    }

//    std::cout << "checking front of cols needs to be expanded" << std::endl;
    // expand front col
    for (int i = 0; i < (int) grid.size(); i++){
        if (grid.at(i).at(0) && grid.at(i).size() <= MAX_COLS){
            for (int j = 0; j < (int) grid.size(); j++){
                grid.at(j).insert(grid.at(j).begin(), nullptr);

            }
//            std::cout << "expanded front of cols" << std::endl;
        }
    }

//    std::cout << "checking back of cols needs to be expanded" << std::endl;
    // expand back col
    for (int i = 0; i < (int) grid.size(); i++){
        if (grid.at(i).at(grid.at(i).size() - 1) && (int) grid.at(i).size() <= MAX_COLS){
            for (int j = 0; j < (int) grid.size(); j++){
                grid.at(j).insert(grid.at(j).end(), nullptr);
            }
//            std::cout << "expanded back of cols" << std::endl;
        }
    }
}
