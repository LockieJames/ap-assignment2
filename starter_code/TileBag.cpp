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
#define MAX_CHOICE 3

TileBag::TileBag() {
    tileBag = new LinkedList();
    random = 0;
    // Seed used for randomness
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

// Not the most efficient, but does allow for code reusability
void TileBag::sortMethod(Tile* tile) {
    random = rand() % MAX_CHOICE;
    addRandomTile(random, tile);
}

// Not the most efficient, but does allow for code reusability
void TileBag::shuffleBag() {
    int counter = 0;
    
    while (counter < tileBag->size()) {
        random = rand() % MAX_CHOICE;
        Tile* getTile = tileBag->get(counter);
        
        addRandomTile(random, getTile);
        
        tileBag->deleteAtIndex(counter);
        
        counter++;
    }
}

void TileBag::addRandomTile(int random, Tile* tile) {
    if (random == 0) {
        tileBag->addFront(tile);
    } else if (random == 1) {
        tileBag->addEnd(tile);
    } else if (random == 2) {
        tileBag->addMid(tile);
    }
}

// For testing purposes
void TileBag::showBag() {
    std::cout << "--------" << std::endl;
    std::cout << "Tiles currently in bag:" << "\n" << std::endl;
    
    for (int i = 0; i < tileBag->size(); i++) {
        std::cout << tileBag->get(i)->getColour();
        std::cout << tileBag->get(i)->getShape();
    
        if ((i % 8) == 7) {
            std::cout << std::endl;
        } else {
            std::cout << " ";
        }
    }
    
    std::cout << std::endl << "--------" << std::endl;
}

int TileBag::size() {
    return getTileBag()->size();
}

LinkedList* TileBag::getTileBag() {
    return tileBag;
}
