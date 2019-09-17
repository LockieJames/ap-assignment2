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
        void addBack(Tile* tile);
        void deleteFront();

    private:
        Node* head;
        Node* tail;
        int currPos;
};

#endif // ASSIGN2_LINKEDLIST_H
