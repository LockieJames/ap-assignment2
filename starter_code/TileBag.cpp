//
//  TileBag.cpp
//  
//
//  Created by Peter Bui on 22/9/19.
//

#include "TileBag.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

#define MAX_SHUFFLE 2
#define MAX_ADD 3

TileBag::TileBag() {
    tileBag = new LinkedList();
    srand(time(0));
    makeBag();
}

TileBag::~TileBag() {
    
}

void TileBag::createTiles() {
    for (int i = 0; i < PART_LENGTH; i++) {
        for (int j = 0; j < PART_LENGTH; j++) {
            Tile* tile = new Tile(colours[i], shapes[j]);
            sortMethod(tile);
        }
    }
}

void TileBag::makeBag() {
    int create = 0;
    int shuffle = 0;

    while (create < MAX_SHUFFLE) {
        createTiles();
        create++;
    }

    while (shuffle < MAX_SHUFFLE) {
        shuffleBag();
        shuffle++;
    }
}

void TileBag::sortMethod(Tile* tile) {
    int random = rand() % MAX_ADD;

    if (random == 0) {
        tileBag->addFront(tile);
    } else if (random == 1) {
        tileBag->addEnd(tile);
    } else if (random == 2) {
        tileBag->addMid(tile);
    }
}

void TileBag::shuffleBag() {
    int counter = 0;
    int random = rand() % MAX_ADD;
    
    while (counter < tileBag->size()) {
        Tile* getTile = tileBag->get(counter);
        
        if (random == 0) {
            tileBag->addFront(getTile);
        } else if (random == 1) {
            tileBag->addEnd(getTile);
        } else if (random == 2) {
            tileBag->addMid(getTile);
        }
        
        tileBag->deleteAtIndex(counter);
        
        counter++;
    }
}

// For testing purposes
void TileBag::showBag() {
    std::cout << "--------" << std::endl;
    std::cout << "Tiles currently in bag:" << std::endl;
    
    for (int i = 0; i < tileBag->size(); i++) {
        std::cout << tileBag->get(i)->getColour();
        std::cout << tileBag->get(i)->getShape() << " ";
    
        if ((i % 8) == 7) {
            std::cout << std::endl;
        }
    }
    
    std::cout << std::endl << "--------" << std::endl;
}

LinkedList* TileBag::getTileBag() {
    return tileBag;
}

//std::cout << "--------" << std::endl;
//
//for (int i = 0; i < tileBag->size(); i++) {
//    std::cout << tileBag->get(i)->getColour();
//    std::cout << tileBag->get(i)->getShape() << ", ";
//
//    if (i == 35 || i == 71 || i == 107) {
//        std::cout << std::endl;
//    }
//}
//
//std::cout << "--------" << std::endl;
