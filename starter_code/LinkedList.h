#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"
#include "Tile.h"

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
        void deleteFront();
        void deleteAtIndex(int i);

    private:
        Node* head;
        Node* tail;
        int numTiles;
};

#endif // ASSIGN2_LINKEDLIST_H
