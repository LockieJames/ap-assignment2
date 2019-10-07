#include "LinkedList.h"
#include <iostream>

LinkedList::LinkedList()
{
    head = nullptr;
    tail = nullptr;
}

LinkedList::~LinkedList()
{
    clear();
}

int LinkedList::size() {
    int counter = 0;
    Node* curr = head;
    
    while (curr != nullptr) {
        curr = curr->next;
        counter++;
    }
    
    return counter;
}

void LinkedList::clear() {
    while (head != nullptr) {
        Node* nNode = head;
        head = head->next;
        delete nNode->tile;
        delete nNode;
    }
    
    tail = nullptr;
    
    delete head;
    delete tail;
}

Tile* LinkedList::get(int i) {
    Tile* cTile = nullptr;
    
    if (i > -1 && i < size()) {
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
        nNode->next = head;
        head = nNode;
    }
}

void LinkedList::addEnd(Tile* tile) {
    Node* nNode = new Node(tile, nullptr, tail);
    
    if (head == nullptr) {
        head = nNode;
    } else {
        Node* travel = head;
        while (travel->next != nullptr) {
            travel = travel->next;
        }
        travel->next = nNode;
        nNode->prev = travel;
        tail = nNode;
    }
}

void LinkedList::addMid(Tile* tile) {
    Node* nNode = new Node(tile, nullptr, nullptr);
    int counter = 0;

    if (head == nullptr) {
        head = nNode;
    } else if (size() < BEFORE_MID) {
        addEnd(tile);
    } else {
        int middle = (size() / 2) - 1;
        
        Node* travel = head;

        while (travel->next != nullptr && counter != middle) {
            travel = travel->next;
            counter++;
        }

        nNode->prev = travel;
        nNode->next = travel->next;
        
        // In case there is a nullptr somehow
        if (travel->next != nullptr) {
            travel->next->prev = nNode;
            travel->next = nNode;
        }
    }
}

void LinkedList::deleteFront() {
    if (head != nullptr) {
        Node* curr = head;
        head = head->next;
        head->prev = nullptr;
        delete curr;
    }
    if (head == nullptr) {
        tail = nullptr;
    }
}

Tile* LinkedList::removeTile(char colour, int shape)
{
    Tile* requiredTile = nullptr;
    Node* curr = head;
    int counter = 1;

    if(head != nullptr)
    {
        while(curr != nullptr && requiredTile == nullptr)
        {
            if(curr->tile->getShape() == shape && curr->tile->getColour() == colour)
            {
                char nColour = curr->tile->getColour();
                int nShape = curr->tile->getShape();

                requiredTile = new Tile(nColour, nShape);
                
                if (curr->prev != nullptr) {
                    curr->prev->next = curr->next;
                    
                    if (curr->next != nullptr) {
                        curr->next->prev = curr->prev;
                    }
                    delete curr;
                } else {
                    deleteFront();
                }
            }
            counter++;
            curr = curr->next;
        }
    }
    return requiredTile;
}

// If colour prints colourful result
std::string LinkedList::getTiles(bool colour) {
    Menu menu;
    std::string allTiles = "";
    Node* curr = head;
    while (curr != nullptr) {
        if (colour) {
            allTiles += menu.printColour(curr->tile->getColour(), std::cout);
        } else {
            allTiles += curr->tile->getColour();
        }

        allTiles += std::to_string(curr->tile->getShape());
        if (curr->next != nullptr)
            allTiles += ",";
        curr = curr->next;
    }
    return allTiles;
}

void LinkedList::clearTiles() {
    Node* curr = head;
    int counter = 0;
    
    while (counter < size()) {
        delete curr->tile;
        curr = curr->next;
        counter++;
    }
}
