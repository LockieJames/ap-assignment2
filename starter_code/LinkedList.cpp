#include "LinkedList.h"
#include "Menu.h"
#include <iostream>

#define PART_LENGTH 6
#define BEFORE_MID 4

LinkedList::LinkedList()
{
    head = nullptr;
    tail = nullptr;
}

LinkedList::~LinkedList()
{
//    std::cout << "Got to start delete list" << std::endl;
    clear();
//    std::cout << "Got to end delete list" << std::endl;
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
        
        // travel->next != nullptr &&
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

// Can change to be more efficient/less complex
//void LinkedList::deleteAtIndex(int i) {
//    if (i > -1 && i < size()) {
//        Node* curr = head;
//        // For deleting at specific index
//        Node* prev = nullptr;
//        int count = 0;
//
//        std::cout << "Index: " << i << std::endl;
//
//        while (curr != nullptr && count < i) {
//            prev = curr;
//            curr = curr->next;
//            count++;
//        }
//
//        std::cout << "Count: " << count << std::endl;
//
//        if (count == i) {
//            if (i == 0 || prev == nullptr) {
//                deleteFront();
//            } else {
//                prev->next = curr->next;
//                delete curr;
//            }
//        }
//        else {
//            throw std::runtime_error("Not Deleted: Index could not be found");
//        }
//    } else {
//        throw std::out_of_range("Not Deleted: Provided index not acceptable");
//    }
//}

Tile* LinkedList::removeTile(char colour, int shape)
{
    Tile* requiredTile = nullptr;
    Node* curr = head;
    int counter = 1;
    
//    std::cout << "Got to remove tile" << std::endl;

    if(head != nullptr)
    {
//        std::cout << "Got to head != nullptr" << std::endl;
        
        while(curr != nullptr && requiredTile == nullptr)
        {
            if(curr->tile->getShape() == shape && curr->tile->getColour() == colour)
            {
                char nColour = curr->tile->getColour();
                int nShape = curr->tile->getShape();
                
//                std::cout << "Tile is at: " << counter << std::endl;
//
//                std::cout << "Got to place tile nColour: " << nColour << std::endl;
//                std::cout << "Got to place tile nShape: " << nShape << std::endl;

                requiredTile = new Tile(nColour, nShape);
                
                // Rework node workings
                if (curr->prev != nullptr) {
//                    std::cout << "Got to curr->prev != nullptr" << std::endl;
                    
//                    std::cout << "Got to curr->prev tile Colour: " << curr->prev->tile->getColour() << std::endl;
//                    std::cout << "Got to curr->prev tile Shape: " << curr->prev->tile->getShape() << std::endl;
                    
                    curr->prev->next = curr->next;
                    
                    if (curr->next != nullptr) {
                        curr->next->prev = curr->prev;
//                        std::cout << "Got to curr->next tile Colour: " << curr->next->tile->getColour() << std::endl;
//                        std::cout << "Got to curr->next tile Shape: " << curr->next->tile->getShape() << std::endl;
                    }
                    
//                    std::cout << "Got to after curr tile colour: " << curr->tile->getColour() << std::endl;
//                    std::cout << "Got to after curr tile shape: " << curr->tile->getShape() << std::endl;
                    
                    delete curr;
                } else {
//                    std::cout << "Got to place tile deleteFront" << std::endl;
                    deleteFront();
                }
            }
            counter++;
            curr = curr->next;
        }
    }
    
//    std::cout << "tile in required tile colour: " << requiredTile->getColour() << std::endl;
//    std::cout << "tile in required tile shape: " << requiredTile->getShape() << std::endl;
    
    return requiredTile;
}

// Returns true if the tile exists in the linked list, else false
//bool LinkedList::findNodeByTile(Tile* tile)
//{
//    bool found = false;
//
//    Node* curr = head;
//
//    if(head != nullptr)
//    {
//        while(curr != nullptr && !found)
//        {
//            if(curr->tile == tile)
//            {
//                found = true;
//            }
//
//            curr = curr->next;
//        }
//    }
//
//    return found;
//}
//
//int LinkedList::getIndexByTile(Tile* tile)
//{
//    bool found = false;
//    int index = 0;
//
//    Node* curr = head;
//
//    if(head != nullptr)
//    {
//        while(curr != nullptr && !found)
//        {
//            if(curr->tile == tile)
//            {
//                found = true;
//            }
//
//            curr = curr->next;
//            index++;
//        }
//    }
//
//    return index;
//}

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
//        std::cout << "Tile " << counter << " deleted" << std::endl;
    }
}
