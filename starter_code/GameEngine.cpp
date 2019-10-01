//
//  GameEngine.cpp
//
//
//  Created by Louis Baudinette on 24/09/2019
//

#include "GameEngine.h"

GameEngine::GameEngine(int numPlayers){
    this->tileBag = TileBag();
    this->gameBoard = Board();
    this->menu = Menu();

    for (int i = 0; i < numPlayers; i++){
        this->players.push_back(Player());
    }

    menu.startMessage();
    menu.menuOptions();
}

GameEngine::~GameEngine(){

}

// TODO: implement methods for game states and player turns
void GameEngine::gameInit(){
    // create ordering of tile bag
    this->tileBag.makeBag();

    // TODO: get hands for each player from tileBag and assign to players

    // begin game loop
    gameLoop();

}

void GameEngine::gameLoop(){
    bool gameFinished = false;

    while (!gameFinished){
        for(int i = 0; i < players.size(); i++){
            bool turnComplete = false;
            while (!turnComplete){
                // TODO: action - place tile

                // TODO: action - replace tile

                // TODO: action - save game

                // TODO: action - quit game
            }

            // check if game end conditions are fulfilled
            if(gameEndCheck()){
                gameFinished = true;
            }
        }
    }

    gameFinish();

}

void GameEngine::gameFinish(){
    // TODO: events upon game finish
}

bool GameEngine::gameEndCheck(){
    bool returnVal = false;

    // TODO: tileBag.size() -- size() isn't a method in tileBag
    // check current size of tileBag
//    if (this->tileBag.size() == 0){
//        for (auto i : this->players){
//            if (i.getHandSize() == 0){
//                returnVal = true;
//            }
//        }
//    }

    return returnVal;
}
