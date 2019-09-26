
#ifndef ASSIGN2_BOARD_H
#define ASSIGN2_BOARD_H

#include <vector>
#include "TileCodes.h"

#define ROWS    6
#define COLS    8 / 2
#define PATTERN " | "
#define BORDER_PATTERN  "-----"
#define SPACE   "   "
#define A_VALUE 65

class Tile;

class Board {
public:
    Board();
    ~Board();
    bool printBoard();
    bool placeTile(Tile &tile, char row, int col);

private:
    void printBorder();
    void printCoord(int startNumber);

    int validateRow(int colourShape, int row, int col, int rowDirection, bool right);
    bool isEmpty();

    std::vector<std::vector<Tile*> > grid;
    bool emptyBoard = true;
};


#endif //ASSIGN2_BOARD_H
