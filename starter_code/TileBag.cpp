//
//  TileBag.cpp
//  
//
//  Created by Peter Bui on 22/9/19.
//

#include "TileBag.h"
#include <iostream>

TileBag::TileBag() {
    tileBag = new LinkedList();
}

TileBag::~TileBag() {
    
}

void TileBag::createTiles() {
    for (int i = 0; i < PART_LENGTH; i++) {
        for (int j = 0; j < PART_LENGTH; j++) {
            Tile* tile = new Tile(colours[i], shapes[j]);
            tileBag->addEnd(tile);
        }
    }
}

void TileBag::makeBag() {
    int counter = 0;
    
    while (counter < 3) {
        createTiles();
        counter++;
    }
}

void TileBag::sortBag() {
    
}

void TileBag::sortMethod() {
    
}
