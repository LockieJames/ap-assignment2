
#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include <vector>
#include <string>
#include "Node.h"
#include "Tile.h"
#include "Menu.h"

#define PART_LENGTH 6
#define BEFORE_MID 4

class LinkedList
{
    public:
        LinkedList();
        ~LinkedList();
    
        int size();
        void clear();
        Tile* get(int i);
        void addFront(Tile* tile);
        void addEnd(Tile* tile);
        void addMid(Tile* tile);
        void deleteFront();
        void deleteAtIndex(int i);
        bool findNodeByTile(Tile* tile);
        int getIndexByTile(Tile* tile);
        std::string getTiles(bool colour, bool shape);
        Tile* removeTile(char colour, int shape);
        void clearTiles();

    private:
        Node* head;
        Node* tail;
};

#endif // ASSIGN2_LINKEDLIST_H
