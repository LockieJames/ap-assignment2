#include "LinkedList.h"
#include <iostream>

LinkedList::LinkedList()
{
    head = nullptr;
    tail = nullptr;
    currPos = 0;
}

LinkedList::~LinkedList()
{
    
}

int LinkedList::size() {
    return currPos;
}

void LinkedList::clear() {
    while (currPos > 0) {
        deleteFront();
    }
}

Tile* LinkedList::get(int i) {
    Tile* cTile = new Tile('Z', 99);
    
    if (i > -1) {
        Node* curr = head;
        int count = 0;
        
        while (curr != nullptr && count < i) {
            curr = curr->next;
            count++;
        }
        
        if (count == i) {
            cTile = curr->tile;
        }
        else {
            // Replace with exception later
            std::cout << "Couldn't find" << std::endl;
        }
    } else {
        // Replace with exception later
        std::cout << "Not valid" << std::endl;
    }
    
    return cTile;
}

void LinkedList::addFront(Tile* tile) {
    Node* nNode = new Node(tile, head);
    head = nNode;
    currPos++;
}

void LinkedList::addBack(Tile* tile) {
    tail = nullptr;
    Node* nNode = new Node(tile, tail);
    
    if (head == nullptr) {
        head = nNode;
    } else {
        tail = head;
        while (tail->next != nullptr) {
            tail = tail->next;
        }
        tail->next = nNode;
    }
    currPos++;
}

void LinkedList::deleteFront() {
    if (head != nullptr) {
        Node* nNode = head;
        head = head->next;
        delete nNode;
        currPos--;
    }
}
