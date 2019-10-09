
#ifndef TileBag_h
#define TileBag_h

#include "LinkedList.h"
#include "Tile.h"
#include <stdlib.h>
#include <time.h>

#define PART_LENGTH 6
#define MAX_CREATE 2
#define MAX_SHUFFLE 5
#define MAX_CHOICE 3
#define MAX_CHOICE_2 2

class TileBag {
    public:
        TileBag();
        TileBag(LinkedList* loadedTileBag);
        ~TileBag();
        void makeBag();
        void createTiles();
        void sortMethod(Tile* tile);
        void shuffleBag();
        void addRandomTile(int random, Tile* tile);
        void showBag();
        int size();
        Tile* getFront();
        LinkedList* getTileBag();
    
    private:
        Colour colours[PART_LENGTH] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};
        Shape shapes[PART_LENGTH] = {CIRCLE, STAR_4, DIAMOND, SQUARE, STAR_6, CLOVER};
        LinkedList* tileBag;
        int random;
};

#endif /* TileBag_h */
