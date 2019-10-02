#include "LinkedList.h"
#include <iostream>

#define PART_LENGTH 6

LinkedList::LinkedList()
{
    head = nullptr;
    tail = nullptr;
    numTiles = 0;
}

LinkedList::~LinkedList()
{
    clear();
}

int LinkedList::size() {
    return numTiles;
}

void LinkedList::clear() {
    while (numTiles > 0) {
        deleteFront();
    }
    tail = nullptr;
}

Tile* LinkedList::get(int i) {
    Tile* cTile = nullptr;
    
    if (i > -1 && i < numTiles) {
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
            throw std::runtime_error("Not Found: Index could not be found");
        }
    } else {
        throw std::out_of_range("Not Found: Provided index not acceptable");
    }
    
    return cTile;
}

void LinkedList::addFront(Tile* tile) {
    Node* nNode = new Node(tile, head, nullptr);
    
    if (head == nullptr) {
        head = nNode;
    }
    else {
        head->prev = nNode;
        head = nNode;
    }
    numTiles++;
}

void LinkedList::addEnd(Tile* tile) {
    Node* nNode = new Node(tile, nullptr, tail);
    
    if (head == nullptr) {
        head = nNode;
    } else {
        tail = head;
        while (tail->next != nullptr) {
            tail = tail->next;
        }
        tail->next = nNode;
    }
    numTiles++;
}

void LinkedList::addMid(Tile* tile) {
    Node* nNode = new Node(tile, nullptr, nullptr);
    int counter = 0;

    if (head == nullptr) {
        head = nNode;
    } else if (numTiles < 4) {
        addEnd(tile);
        numTiles--;
    } else {
        int middle = numTiles / 2;
        
        Node* currNode = head;
        
        while (currNode->next != nullptr && counter != middle) {
            currNode = currNode->next;
            counter++;
        }
        
        nNode->prev = currNode;
        nNode->next = currNode->next;
        
        if (currNode->next->next != nullptr) {
            currNode->next = nNode;
            currNode->next->next->prev = nNode;
        } else {
            currNode->next = nNode;
        }
    }
    numTiles++;
}

void LinkedList::deleteFront() {
    if (head != nullptr) {
        Node* nNode = head;
        head = head->next;
        delete nNode;
        numTiles--;
    }
    if (head == nullptr) {
        tail = nullptr;
    }
}

// Can change to be more efficient/less complex
void LinkedList::deleteAtIndex(int i) {
    if (i > -1 && i < numTiles) {
        Node* curr = head;
        // For deleting at specific index
        Node* prev = nullptr;
        int count = 0;
        
        while (curr != nullptr && count < i) {
            prev = curr;
            curr = curr->next;
            count++;
        }
        
        if (count == i) {
            if (i == 0 || prev == nullptr) {
                deleteFront();
            } else {
                prev->next = curr->next;
                delete curr;
                numTiles--;
            }
        }
        else {
            throw std::runtime_error("Not Deleted: Index could not be found");
        }
    } else {
        throw std::out_of_range("Not Deleted: Provided index not acceptable");
    }
}

// Returns true if the tile exists in the linked list, else false
bool LinkedList::findNodeByTile(Tile* tile)
{
    bool found = false;
    
    Node* curr = head;
    
    if(head != nullptr)
    {
        while(curr != nullptr && !found)
        {
            if(curr->tile == tile)
            {
                found = true;
            }
            
            curr = curr->next;
        }
    }
    
    return found;
}

Tile* LinkedList::removeTile(char colour, int shape)
{
    Tile* requiredNode = nullptr;
    Node* curr = head;

    if(head != nullptr)
    {
        while(curr != nullptr && requiredNode == nullptr)
        {
            if(curr->tile->getShape() == shape && curr->tile->getColour() == colour)
            {
                requiredNode = curr->tile;
                curr->prev->next = curr->next;
                delete curr;
            }
            curr = curr->next;
        }
    }
    return requiredNode;
}

int LinkedList::getIndexByTile(Tile* tile)
{
    bool found = false;
    int index = 0;
    
    Node* curr = head;
    
    if(head != nullptr)
    {
        while(curr != nullptr && !found)
        {
            if(curr->tile == tile)
            {
                found = true;
            }
            
            curr = curr->next;
            index++;
        }
    }
    
    return index;
}

std::string LinkedList::getTiles() {
    std::string allTiles = "";
    Node* curr = head;
    while (curr != nullptr) {
        allTiles += curr->tile->getColour() << curr->tile->getShape();
        if (curr->next != nullptr)
            allTiles += ",";
        curr = curr->next;
    }
    return allTiles;
}