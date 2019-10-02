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
    bool quit = false;

    while (!gameFinished){
        for(int i = 0; i < players.size(); i++){
            bool turnComplete = false;
            while (!turnComplete){
                
                if (playerAction == 0){
                    // TODO: action - place tile
                    placeTile(players.at(i), );
                } else if (playerAction == 1){
                    // TODO: action - replace tile
                    replaceTile(players.at(i), );
                } else if (playerAction == 2){
                    // TODO: action - save game
                    saveGame();
                } else if (playerAction == 3){
                    // action - quit game
                    quit = true;
                    turnComplete = true;
                    gameFinished = true;
                    // print exit statement
                }

            }

            // check if game end conditions are fulfilled
            if(gameEndCheck()){
                gameFinished = true;
            }
        }
    }

    if (quit){
        // TODO: quit game
    } else (
        // TODO: finish game
        gameFinish();
    )

}

void GameEngine::gameFinish(){
    // TODO: events upon game finish
}

bool GameEngine::gameEndCheck(){
    bool returnVal = false;

    // TODO: tileBag.size() -- size() isn't a method in tileBag
    // check current size of tileBag
    // if (this->tileBag.size() == 0){
    //     for (auto i : this->players){
    //         if (i.getHandSize() == 0){
    //              returnVal = true;
    //         }
    //     }
    // }

    return returnVal;
}

bool GameEngine::placeTile(Player player, Colour colour, Shape shape, char rowInput, int colInput){
    // placeholder logic/functions/exceptions
    // TODO: add appropriate menu callbacks to print info to console if need be
    try {
        // get tile to be placed from player's hand
        Tile fromPlayer = player.getTileFromHand(colour, shape);

        // place tile on board, getting points from tile placement for player
        int points = gameBoard.placeTile(fromPlayer, rowInput, colInput);

        // add points to player's score
        player.addScore(points);

        // get new tile from tilebag for player
        Tile forPlayer = tileBag.getTile();

        // add new tile to player's hand
        player.addTileToHand(forPlayer);

    } catch (exception& tileNotInHand){
        // if tile does not exist in player's hand
        // print appropriate error message

    } catch (exception& invalidTilePlacement){
        // if tile cannot be placed on board in given position
        // add tile back to hand
        player.addTileToHand(fromPlayer);
        // print appropriate error message
    }

}

bool GameEngine::replaceTile(Player player, Colour colour, Shape shape){
    // placeholder logic/functions/exceptions
    // TODO: add appropriate menu callbacks to print info to console if need be
    try{
        // get tile from player's hand
        Tile fromPlayer = player.getTileFromHand(colour, shape);

        // get tile for player from tilebag
        Tile forPlayer = tileBag.getTile();

        // add tile from player to tilebag
        tileBag.addTile(fromPlayer);

        // add tile from tilebag to player's had
        player.addTileToHand(forPlayer);

    } catch (exception& tileNotInHand){
        // if tile does not exist in player's hand
        // print appropriate error message

    }
}

bool GameEngine::saveGame(){
    
}
