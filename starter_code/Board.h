
#ifndef ASSIGN2_BOARD_H
#define ASSIGN2_BOARD_H

#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include "TileCodes.h"
#include "Tile.h"

#define ONE_ROW 1
#define TWO_ROWS 2
#define QWIRKLE 5
#define ROWS    6
#define COLS    8 / 2
#define MAX_ROWS    26
#define MAX_COLS    26 / 2
#define PATTERN " | "
#define BORDER_PATTERN  "-----"
#define BORDER_PATTERN_END  "----"
#define SPACE   "   "
#define A_VALUE 65

class Tile;

class Board {
public:
    Board();
    Board(std::vector<std::vector<Tile*>> loadedGrid, bool firstRowOffset);
    ~Board();
    bool printBoard(std::ostream &destination);
    int placeTile(Tile &tile, char row, int col);

private:
    void printBorder(std::ostream &destination);
    void printCoord(std::ostream &destination, int startNumber);

    int validateRow(int colourShape, int row, int col, int rowDirection, bool right);
    bool isEmpty();
    bool edgeRow(int row, int rowDirection);
    bool edgeCol(int col, bool odd, bool right);
    int calculateCol(bool odd, bool right, int col);
    std::map<std::string, int> getMap(int shapeColour, int row, int col);
    void expandBoard();

    std::vector<std::vector<Tile*>> grid;
    bool emptyBoard = true;
    int scoreTurn;
    bool firstRowOffset;
};


#endif //ASSIGN2_BOARD_H
