#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include <vector>
#include "Node.h"
#include "Tile.h"

#define PART_LENGTH 6

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

    private:
        Node* head;
        Node* tail;
        int numTiles;
};

#endif // ASSIGN2_LINKEDLIST_H
