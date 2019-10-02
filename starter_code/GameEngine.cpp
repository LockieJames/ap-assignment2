//
//  GameEngine.cpp
//
//
//  Created by Louis Baudinette on 24/09/2019
//

#include <regex>
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

    while (!gameFinished) {
        for (int i = 0; i < players.size(); i++) {
            printGameInfo(i);
            bool turnComplete = false;
            while (!turnComplete) {

                // TODO: Ask for input in menu and pass string userInput
                quit = true;
                turnComplete = true;
                gameFinished = true;
                // print exit statement
                std::string userInput;
                std::smatch field;
                std::cout << "> ";
                std::getline(std::cin, userInput);

                if (std::regex_match(userInput, field, std::regex(R"(place\s[ROYGBP][1-6]\sat\s[A-F][0-7])"))) {
                    turnComplete = placeTile(players.at(i), field[0].str()[6], field[0].str()[7],
                                             field[0].str()[12], field[0].str()[13]);
                    if (tileBag.size() == 0 && players.at(i).getHand()->size() == 0) {
                        gameFinished = true;
                    }
                } else if (std::regex_match(userInput, field, std::regex(R"(replace\s[ROYGBP][1-6])"))) {
                    turnComplete = replaceTile(players.at(i), field[0].str()[8], field[0].str()[9]);
                // }  else if (playerAction == 2) {
                    // TODO: action - save game
                    // saveGame();
                // } else if (playerAction == 3) {
                    // action - quit game
                } else {
                    std::cout << "Invalid input!" << std::endl;
                }

                if (!turnComplete)
                    std::cout << "Couldn't place tile or replace tile!" << std::endl;
            }
        }
    }
    gameFinish();
}

void GameEngine::printGameInfo(int playerIndex) {
    std::cout << players[playerIndex].getName() << ", it's your turn" << std::endl;
    for (Player player : players) {
        std::cout << "Score for " << player.getName() << " is: " << player.getScore() << std::endl;
    }
    gameBoard.printBoard(std::cout) ;
    std::cout << "Your hand is " << std::endl;
    std::cout << players[playerIndex].getHand()->getTiles() << std::endl;
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

bool GameEngine::placeTile(Player player, Colour colour, Shape shape, char rowInput, int col) {
    // TODO: add appropriate menu callbacks to print info to console if need be

    bool placed = false;
    Tile* tile = player.getHand()->removeTile(colour, shape);
    if (tile != nullptr) {
        int score = gameBoard.placeTile(*tile, rowInput, col);
        if (score != 0) {
            player.addScore(score);
            drawTile(player);
            placed = true;
        } else {
            player.getHand()->addFront(tile);
            // IF throw an exception that tile wasnt placed then goes HERE
            /*
             * catch (exception& invalidTilePlacement){
             * if tile cannot be placed on board in given position
             * print appropriate error message
             */

        }
    }
    // If throw exeption that tile wasnt found then else statement and goes HERE
    /*
     * catch (exception& tileNotInHand){
          if tile does not exist in player's hand
          print appropriate error message
     */

    return  placed;
}

void GameEngine::drawTile(Player player) {
    if (tileBag.size() != 0) {
        player.getHand()->addEnd(tileBag.getTileBag()->get(0));
        tileBag.getTileBag()->deleteFront();
    }
}

bool GameEngine::replaceTile(Player player, Colour colour, Shape shape){
    // TODO: add appropriate menu callbacks to print info to console if need be
    bool placed = false;
    // remove a tile from players  hand
    Tile* tile = player.getHand()->removeTile(colour, shape);
    if (tile != nullptr) {
        // draw a tile from bag
        player.getHand()->addEnd(tileBag.getTileBag()->get(0));

        // add player's tile to the bag
        tileBag.getTileBag()->addEnd(tile);

        // delete drawn node from tile bag
        tileBag.getTileBag()->deleteFront();
        placed = true;
    }
    return placed;
}

bool GameEngine::saveGame() {
    return false;
}
