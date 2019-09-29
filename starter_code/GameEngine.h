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
    GameEngine(int numPlayers);
    ~GameEngine();

    // todo: implement methods for game states and player turns
    void gameInit();

    private:
    Board gameBoard;
    TileBag tileBag;
    Menu menu;
    std::vector<Player> players;

    void gameLoop();
    void GameEngine::gameFinish();
    std::vector<int> calcMaxTileSeq(LinkedList* hand);
    void openingHelper(std::vector<int> openingPlay, int PlayerIndex);
    bool GameEngine::gameEndCheck();

};

#endif /* GameEngine_h */
