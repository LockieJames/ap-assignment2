#include "LinkedList.h"

#include <iostream>

#define EXIT_SUCCESS    0

void listTest();

int main(void)
{
//    LinkedList* list = new LinkedList();

    listTest(); // For testing adding to front and back of list
    
    std::cout << "TODO: Implement Qwirkle!" << std::endl;

    return EXIT_SUCCESS;
}

void listTest() {
    LinkedList* list = new LinkedList();
    
    // Testing for nodes placement
    Tile* tile1 = new Tile('R', 1);
    Tile* tile2 = new Tile('O', 2);
    Tile* tile3 = new Tile('Y', 3);
    Tile* tile4 = new Tile('G', 4);
    Tile* tile5 = new Tile('B', 5);
    
    list->addFront(tile1);
    list->addFront(tile2);
    list->addFront(tile3);
    list->addFront(tile4);
    list->addFront(tile5);
    
//    list->addBack(tile1);
//    list->addBack(tile2);
//    list->addBack(tile3);
//    list->addBack(tile4);
//    list->addBack(tile5);

    for (int i = 0; i < list->size(); i++) {
        std::cout << list->get(i)->getColour() << ", ";
        std::cout << list->get(i)->getShape() << std::endl;
    }
    
    delete list;
}
