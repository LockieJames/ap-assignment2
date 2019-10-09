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

/*
 * Function goes throughout the linked list and returns a counter of
 * all the nodes within the list.
 */
int LinkedList::size() {
    int counter = 0;
    Node* curr = head;
    
    while (curr != nullptr) {
        curr = curr->next;
        counter++;
    }
    
    return counter;
}

/*
 * Function is responsible for clearing all the tiles and nodes within the
 * list. Called by the destructor.
 */
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

/*
 * Gets a tile at a specific index dependent on the parameter parsed into
 * the function. Throws an exception if the specific tile is not found.
 */
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

/*
 * Adds a tile to the front of the list through the creation of a new
 * node and passing the attributes necessary to create a new node. Access
 * through the head of the list.
 */
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

/*
 * Adds a tile to the end of the list through the creation of a new
 * node and passing the attributes necessary to create a new node.
 * Another node is used to travel through the list to the tail, where
 * the new node is added to the end.
 */
void LinkedList::addEnd(Tile* tile) {
    Node* nNode = new Node(tile, nullptr, tail);
    
    if (head == nullptr) {
        head = nNode;
        head->next = tail;
    } else if (tail == nullptr) {
        tail = nNode;
        head->next = tail;
        tail->prev = head;
    } else {
//        Node* travel = head;
//        while (travel->next != nullptr) {
//            travel = travel->next;
//        }
//        travel->next = nNode;
//        nNode->prev = travel;
//        tail = nNode;
        tail->next = nNode;
        nNode->prev = tail;
        tail = nNode;
    }
}

/*
 * Adds a tile to the middle of the list through the creation of a new
 * node and passing the attributes necessary to create a new node. addMid
 * is not fully called until there are 4 nodes within the list through ease
 * of use, and instead addEnd is called instead to fulfill that addition.
 */
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

/*
 * Deletes the front node/head node, and making the next node the new
 * head node.
 */
void LinkedList::deleteFront() {
    if (head != nullptr) {
        Node* curr = head;
        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        }
        delete curr;
    }
//    if (head == nullptr) {
//        tail = nullptr;
//    }
}

/*
 * Returns the value of a tile that is required to be removed from the
 * linked list, based on the parameters of the data. The associated node
 * is deleted as well. This function is used for the player's linked list.
 */
Tile* LinkedList::removeTile(char colour, int shape)
{
    Tile* requiredTile = nullptr;
    Node* curr = head;

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
                    } else {
                        tail = curr->prev;
                    }
//                    if (curr->next == nullptr) {
//                        tail = curr->prev;
//                    }
                    
                    delete curr;
                    
                } else {
                    deleteFront();
                }
            }
            curr = curr->next;
        }
    }
    return requiredTile;
}

/*
 * Prints out all the tiles within the linked list as a string return
 * value, where the Menu class handles most standard output.
 */
std::string LinkedList::getTiles(bool colour, bool shape) {
    Menu menu;
    std::string allTiles = "";
    Node* curr = head;
    while (curr != nullptr) {
        char getColour = curr->tile->getColour();
        int getShape = curr->tile->getShape();

        if (colour) {
            allTiles += menu.printColour(getColour, std::cout);
        } else {
            allTiles += curr->tile->getColour();
        }

        if (shape) {
            allTiles += menu.printShape(getShape, std::cout);
        } else {
            allTiles += std::to_string(curr->tile->getShape());
        }

        if (curr->next != nullptr)
            allTiles += ",";
        curr = curr->next;
    }
    return allTiles;
}
