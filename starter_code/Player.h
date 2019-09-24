#ifndef Player_h
#define Player_h

#include "LinkedList.h"
#include "Tile.h"
#include <iostream>

class Player
{
    private:
        std::string name;
        LinkedList* hand;
        int score;
    
    public:
        Player();
        ~Player();
        std::string getName();
        void setName(std::string name);
        int getScore();
        void addScore(int num);
        LinkedList* getHand();
};

#endif
