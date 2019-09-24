//
//  GameEngine.h
//  
//
//  Created by Louis Baudinette on 24/09/2019
//

#ifndef GameEngine_h
#define GameEngine_h

// #includes decide later
#include <iostream>
#include "Board.h"
#include "TileBag.h"
#include "Menu.h"
#include "Player.h"

class GameEngine {
    public:
    GameEngine();
    ~GameEngine();

    private:
    Board gameBoard;
    TileBag tileBag;
    Menu menu;
    Player p1;
    Player p2;
};

#endif /* GameEngine_h */
