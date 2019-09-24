//
//  GameEngine.cpp
//  
//
//  Created by Louis Baudinette on 24/09/2019
//

#include "GameEngine.h"

GameEngine::GameEngine(){
    this->tileBag = TileBag();
    this->gameBoard = Board();
    this->menu = Menu();
    // player class to be implemented
    this->p1 = Player();
    this->p2 = Player();

    tileBag.makeBag();

    menu.startMessage();
    menu.menuOptions();
}

GameEngine::~GameEngine(){

}

// todo: implement methods for game states and player turns
void GameEngine::gameStart(){

}