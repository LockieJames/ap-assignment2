#include <iostream>

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
    for (int i = 0; i < (int) grid.size(); ++i) {
        for (int j = 0; j < (int) grid.at(i).size(); ++j) {
            delete grid[i][j];
        }
    }
}

/*
 * Returns number of points earned in this turn depending on tile placement,
 * which is determined through other functions within this class.
 */
int Board::placeTile(Tile &tile, char rowInput, int col) {
    scoreTurn = 0;
    int row = rowInput - A_VALUE;
    isAlreadyInLine = false;

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


    if (
        row <= (int) grid.size() - 1
        && col <= (int) grid.at(0).size() - 1 && validInput
    ) {
        emptyBoard = isEmpty();
        if (emptyBoard) {
            grid[row][col] = &tile;
            emptyBoard = isEmpty();
            scoreTurn = 1;
        } else {
            std::map<std::string, int> colours = getMap(
                tile.getColour(),
                tile.getShape(),
                row,
                col
            );
            std::map<std::string, int> shapes = getMap(
                tile.getShape(),
                tile.getColour(),
                row,
                col
            );

            int shapesColours[] = {
                colours.at("topRight") + shapes.at("topRight"),
                colours.at("topLeft") + shapes.at("topLeft"),
                colours.at("bottomLeft") + shapes.at("bottomLeft"),
                colours.at("bottomRight") + shapes.at("bottomRight")
            };
            int invalidColourAndShape = INVALID_VALUE + INVALID_VALUE;

            bool offDiagonal = (colours["topRight"] == colours["bottomLeft"])
                || (shapes["topRight"] == shapes["bottomLeft"]);
            bool mainDiagonal = (colours["topLeft"] == colours["bottomRight"])
                || (shapes["topLeft"] == shapes["bottomRight"]);

            int positions = 0;

            bool placeable = true;
            for (int i = 0; i < (int) shapes.size(); ++i) {
                if (shapesColours[i] != 0) {
                    positions++;
                }
                if (
                    (shapesColours[i] == invalidColourAndShape)
                    || isAlreadyInLine
                ) {
                    placeable = false;
                    scoreTurn = 0;
                }
            }

            if (positions == 1 && placeable) {
                grid[row][col] = &tile;
                scoreTurn += ONE_ROW;
            } else if (positions == 2 && placeable) {
                if((shapesColours[0] != 0 && shapesColours[2] != 0)
                   || (shapesColours[1] != 0 && shapesColours[3] !=0)) {
                    if (offDiagonal || mainDiagonal) {
                        grid[row][col] = &tile;
                        scoreTurn += ONE_ROW;
                    }
                } else  {
                    grid[row][col] = &tile;
                    scoreTurn += TWO_ROWS;
                }
            } else if (positions == 3 && placeable) {
                if (offDiagonal || mainDiagonal) {
                    grid[row][col] = &tile;
                    scoreTurn += TWO_ROWS;
                }
            } else if (positions == 4 && placeable){
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

/*
 * Returns the value of the row if valid row, or an invalid value if not
 * allowed to place tile
 */
int Board::validateRow(
    int useColourShape,
    int accompanyingCS,
    int row,
    int col,
    int rowDirection,
    bool right
) {
    int inputColourShape = 0;
    Tile* currentTile;

    bool odd;
    if (!firstRowOffset) {
        odd = row % 2 == 1;
    } else {
        odd = (row + 1) % 2 == 1;
    }

    bool border = (edgeRow(row, rowDirection) || edgeCol(col, odd, right));

    col = calculateCol(odd, right, col);
    row = row + rowDirection;
    odd = !odd;

    // Will hold values for different shapes or colour so they do not repeat
    // twice in one line
    int notMatchingColourShape[] = { 0, 0, 0, 0, 0, 0 };
    int i = 0;

    int numberOfCheckedTiles = 0;
    while (!border && grid[row][col] != nullptr) {
        currentTile = grid[row][col];
        int shape = currentTile->getShape();
        int colour = currentTile->getColour();

        int positionValue = 0;

        if (useColourShape < 7) {
            positionValue = shape;
            notMatchingColourShape[i] = colour;
        } else {
            positionValue = colour;
            notMatchingColourShape[i] = shape;
        }

        if (useColourShape != positionValue){
            inputColourShape = INVALID_VALUE;
        } else {
            inputColourShape = useColourShape;
            numberOfCheckedTiles++;
        }

        border = edgeRow(row, rowDirection) || edgeCol(col, odd, right);
        col = calculateCol(odd, right, col);
        row = row + rowDirection;
        odd = !odd;
        i++;
    }

    // Checks if the value of the tile being placed is already in line
    for (int different : notMatchingColourShape) {
        if (accompanyingCS == different && numberOfCheckedTiles > 0) {
            isAlreadyInLine = true;
        }
    }

    // sets the value of input tile if is valid
    if (inputColourShape != 0 && inputColourShape != INVALID_VALUE) {
        inputColourShape = useColourShape;
    }

    if (!isAlreadyInLine && numberOfCheckedTiles == QWIRKLE) {
        std::cout << "QWIRKLE!!!" << std::endl;
    }

    scoreTurn += numberOfCheckedTiles;

    return inputColourShape;
}

/*
 *
 */
bool Board::edgeRow(int row, int rowDirection) {
    return (row == 0 && rowDirection == -1)
        || (row == (int) grid.size() - 1 && rowDirection == 1);
}

/*
 *
 */
bool Board::edgeCol(int col, bool odd, bool right) {
    return (col == 0 && !odd && !right)
        || (col == (int) grid.at(0).size() - 1 && odd && right);
}

/*
 *
 */
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

/*
 * returns map of neighbouring rows
 */
std::map<std::string, int> Board::getMap(
    int shapeColour,
    int accompanyingCS,
    int row,
    int col
) {
    std::map<std::string, int> coloursShapes;
    coloursShapes.insert(
        std::pair<std::string, int>(
            "topRight",
            validateRow(shapeColour, accompanyingCS, row, col, -1, true)
        )
    );
    coloursShapes.insert(
        std::pair<std::string, int>(
            "bottomRight",
            validateRow(shapeColour, accompanyingCS, row, col, 1, true)
        )
    );
    coloursShapes.insert(
        std::pair<std::string, int>(
            "bottomLeft",
            validateRow(shapeColour, accompanyingCS, row, col, 1, false)
        )
    );
    coloursShapes.insert(
        std::pair<std::string, int>(
            "topLeft",
            validateRow(shapeColour, accompanyingCS, row, col, -1, false)
        )
    );
    return coloursShapes;
}

/*
 * Validating if the board is empty for the first tile
 */
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

/*
 *
 */
bool Board::printBoard(std::ostream &destination, bool symbols) {
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
                    destination << BOARD_PADDING;
                }
                letter++;
            } else {
                destination << PATTERN;
                if (grid[i][j - 1]) {
                    if (symbols){
                        destination << menu.printColour(
                            grid[i][j - 1]->getColour(),
                            destination
                        );
                        destination << menu.printShape(
                            grid[i][j - 1]->getShape(),
                            destination
                        );
                    } else {
                        destination << grid.at(i).at(j - 1)->getColour();
                        destination << grid.at(i).at(j - 1)->getShape();
                    }
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

/*
 *
 */
void Board::printBorder(std::ostream &destination) {
    destination << BOARD_PADDING;
    for (int k = 0; k < (int) grid.at(0).size(); ++k) {
        destination << BORDER_PATTERN;
    }
    destination << BORDER_PATTERN_END;
    destination << std::endl;
}

/*
 *
 */
void Board::printCoord(std::ostream &destination, int startNumber) {
    destination << SPACE;
    if(startNumber % 2 == 1) {
        destination << BOARD_PADDING;
    }
    for (int i = 0; i < (int) grid.at(0).size(); ++i) {
        if (std::to_string(startNumber).length() == 2){
            destination << "   " << startNumber;
        } else {
            destination << "    " << startNumber;
        }
        startNumber += 2;
    }
    destination << std::endl;
}

/*
 *
 */
void Board::expandBoard(){

    // expland front row
    for (auto i : grid.at(0)){
        if (i && (int) grid.size() < MAX_ROWS){
            grid.insert(
                grid.begin(),
                std::vector<Tile *>(grid.at(grid.size() - 1).size())
            );
            firstRowOffset = !firstRowOffset;
        }
    }

    // expand back row
    for (auto i : grid.at(grid.size() - 1)){
        if (i && (int) grid.size() < MAX_ROWS){
            grid.insert(grid.end(), std::vector<Tile *>(grid.at(0).size()));
        }
    }

    // expand front col
    for (int i = 0; i < (int) grid.size(); i++){
        if (grid.at(i).at(0) && grid.at(i).size() < MAX_COLS){
            for (int j = 0; j < (int) grid.size(); j++){
                grid.at(j).insert(grid.at(j).begin(), nullptr);
            }
        }
    }

    // expand back col
    for (int i = 0; i < (int) grid.size(); i++){
        if (
            grid.at(i).at(grid.at(i).size() - 1)
            && (int) grid.at(i).size() < MAX_COLS
        ){
            for (int j = 0; j < (int) grid.size(); j++){
                grid.at(j).insert(grid.at(j).end(), nullptr);
            }
        }
    }
}
