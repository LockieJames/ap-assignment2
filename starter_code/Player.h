
#ifndef Player_h
#define Player_h

#include "LinkedList.h"
#include "Tile.h"
#include <iostream>

class LinkedList;

class Player
{
    private:
        std::string name;
        LinkedList* hand;
        int score;

    public:
        Player();
        Player(std::string loadedName, int loadedScore, LinkedList* loadedHand);
        ~Player();
        std::string getName();
        void setName(std::string name);
        int getScore();
        void addScore(int num);
        LinkedList* getHand();
        int getHandSize();
        void replaceTile(Tile* tile);
        void clearTiles();
};

#endif
