
#ifndef ASSIGN2_BOARD_H
#define ASSIGN2_BOARD_H

#include "Tile.h"

#define ROWS    6
#define COLS    8 / 2
#define PATTERN " | "
#define BORDER_PATTERN  "-----"
#define SPACE   "   "
#define A_VALUE 65

class Board {
public:
    Board();
    ~Board();
    bool printBoard();
    bool placeTile(Tile &tile, char row, int col);

private:
    void printBorder();
    void printCoord(int startNumber);

    bool equalColour(Tile& tile, Tile& otherTile);
    bool equalShape(Tile& tile, Tile& otherTile);
    bool validateField(Tile& tile, Tile& otherTile);
    bool placeTwo(Tile& currentTile, Tile& check1Tile, Tile& check2Tile, int row, int col);

    Tile* grid[COLS][ROWS];
};


#endif //ASSIGN2_BOARD_H
