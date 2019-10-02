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

    std::cout << "Got here1" << std::endl;
    
    for (int i = 0; i < numPlayers; i++) {
        Player* player = new Player();
        this->players.push_back(*player);
        std::cout << "Got here " << i << std::endl;
    }

    std::cout << "Size of Vct: " << players.size() << std::endl;
    std::cout << "Got here2" << std::endl;

    std::cout << "Score at position 0: " << players.at(0).getScore() << std::endl;
    std::cout << "Score at position 1: " << players.at(1).getScore() << std::endl;
    
    menu.startMessage();
    menu.menuOptions();
}

GameEngine::~GameEngine(){
    
}

// TODO: implement methods for game states and player turns
void GameEngine::gameInit(){
    std::vector<int> maxLengthOpening;
    int openingPlayerIndex;

    // TODO: get hands for each player from tileBag and assign to players

    // calculate largest length opening hand and player it belongs to
    for (int i = 0; i < players.size(); i++){
        std::vector<int> currOpening = calcMaxTileSeq(players[i].getHand());
        if (currOpening.size() > maxLengthOpening.size()){
            maxLengthOpening = currOpening;
            openingPlayerIndex = i;
        }
    }
    // make opening play
    openingHelper(maxLengthOpening, openingPlayerIndex);

    // begin game loop
    gameLoop();

}

void GameEngine::gameLoop(){
    bool gameFinished = false;

    while (!gameFinished){
        for(int i = 0; i < players.size(); i++){
            // TODO: action - place tile

            // TODO: action - replace tile

            // TODO: action - save game

            // TODO: action - quit game

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

std::vector<int> GameEngine::calcMaxTileSeq(LinkedList* hand){
    int handSize = hand->size();
    std::vector<int> maxIndexList;

    // iterate through hand, and for each tile, check if other tiles'
    // colour and shape match, and get indexes of tiles
    for (int i = 0; i < handSize; i++){
        Tile* currTile = hand->get(i);
        Colour currColour = currTile->getColour();
        Shape currShape = currTile->getShape();

        std::vector<int> currIndexListColour;
        currIndexListColour.push_back(i);
        std::vector<int> currIndexListShape;
        currIndexListShape.push_back(i);

        for (int j = i; j < handSize; i++){
            if (currColour == hand->get(j)->getColour()){
                currIndexListColour.push_back(j);
            }

            if (currShape == hand->get(j)->getShape()){
                currIndexListShape.push_back(j);
            }
        }

        if (currIndexListColour.size() >= currIndexListShape.size()
            && currIndexListColour.size() > maxIndexList.size()){
            maxIndexList = currIndexListColour;
        }
        else if (currIndexListShape.size() >= currIndexListColour.size()
            && currIndexListShape.size() > maxIndexList.size()){
            maxIndexList = currIndexListShape;
        }
    }

    return maxIndexList;
}

void GameEngine::openingHelper(std::vector<int> openingPlay, int playerIndex){
    // TODO: play opening from p1 based on indexes in p1Opening

    // TODO: make p1 draw p1Opening.size() amount of tiles from tileBag

    // give p1 p1Opening.size() amount of points
    this->players[playerIndex].addScore(openingPlay.size());
}

bool GameEngine::gameEndCheck(){
    bool returnVal = false;

    // check current size of tileBag
    if (this->tileBag.size() == 0){
        for (auto i : this->players){
            if (i.getHandSize() == 0){
                returnVal = true;
            }
        }
    }

    return returnVal;
}
