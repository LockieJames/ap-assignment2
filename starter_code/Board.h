
#ifndef ASSIGN2_BOARD_H
#define ASSIGN2_BOARD_H

#include "Tile.h"

#define ROWS    6
#define COLS    8 / 2
#define PATTERN     " | "
#define BORDER_PATTERN  "-----"
#define SPACE "   "

class Board {
public:
    Board();
    ~Board();
    bool printBoard();

private:
    void printBorder();
    void printCoord(int startNumber);
    Tile* grid[COLS][ROWS];
};


#endif //ASSIGN2_BOARD_H
