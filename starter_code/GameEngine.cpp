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

    std::cout << "Got here1" << std::endl;

    for (int i = 0; i < numPlayers; i++) {
        Player* player = new Player();
        this->players.push_back(player);
        std::cout << "Got here " << i << std::endl;
    }

    std::cout << "Size of Vct: " << players.size() << std::endl;
    std::cout << "Got here2" << std::endl;

    std::cout << "Score at position 0: " << players.at(0)->getScore() << std::endl;
    std::cout << "Score at position 1: " << players.at(1)->getScore() << std::endl;

}

GameEngine::~GameEngine(){
    players.clear();
}

// prints main menu using menu.menuOptions(), gets user's choice, and calls appropriate methods
void GameEngine::mainMenu() {
    menu.startMessage();

    bool validChoice;
    bool quit = false;
    int input;

    while (!quit){
        validChoice = false;
        while (!validChoice){
            menu.menuOptions();
            std::cin >> input;
            if (std::cin.fail() || !(input >= 1 && input <= 4)){
                std::cout << "Input not accepted" << std::endl;
                std::cin.clear();
            } else {
                validChoice = true;
            }
        }

        if (input == 1) {
            newGame();
            quit = true;
        } else if (input == 2) {
            // TODO: action - load game
            menu.loadGame();
            // uncomment the line below when loading is implemented
            // quit = true;
        } else if (input == 3) {
            menu.stuInfo();
        } else if (input == 4) {
            // TODO: action - quit from main menu
            quit = true;
        }
    }

    // call quit method to print exit message as game is closing
    menu.quit();
}

// gets player names and makes tilebag, then starts main game loop
void GameEngine::newGame(){

    menu.newGamePt1();
    for (int i = 0; i < (int) players.size(); i++) {
        menu.newGameNames(i + 1);
        std::string name;
        std::getline(std::cin, name);
        players.at(i)->setName(name);
    }
    menu.newGamePt2();

    tileBag.makeBag();

    gameLoop();
}

// main game loop
void GameEngine::gameLoop(){
    bool gameFinished = false;
    bool gameQuit = false;

    while (!gameFinished) {
        for (int i = 0; i < (int) players.size(); i++) {
            if (!gameQuit){
                printGameInfo(i);
                bool turnComplete = false;
                while (!turnComplete) {

                    // TODO: Ask for input in menu and pass string userInput
                    std::string userInput;
                    std::smatch field;
                    std::cout << "> ";
                    std::getline(std::cin, userInput);

                    if (std::regex_match(userInput, field, std::regex(R"(place\s[ROYGBP][1-6]\sat\s[A-F][0-7])"))) {
                        turnComplete = placeTile(*players.at(i), field[0].str()[6], field[0].str()[7],
                                                field[0].str()[12], field[0].str()[13]);
                        if (tileBag.size() == 0 && players.at(i)->getHand()->size() == 0) {
                            gameFinished = true;
                        }
                    } else if (std::regex_match(userInput, field, std::regex(R"(replace\s[ROYGBP][1-6])"))) {
                        turnComplete = replaceTile(*players.at(i), field[0].str()[8], field[0].str()[9]);
                    }  else if (std::regex_match(userInput, field, std::regex(R"(save\s[a-zA-Z0-9]+)"))) {
                        // TODO: action - save game
                        // saveGame();
                    } else if (userInput == "quit") {
                        turnComplete = true;
                        gameQuit = true;
                        gameFinished = true;
                    } else {
                        std::cout << "Invalid input!" << std::endl;
                    }

                    if (!turnComplete)
                        std::cout << "Couldn't place tile or replace tile!" << std::endl;
                }
            }
        }
    }

    if (!gameQuit){
        // if the game wasn't quit, finish the game normally
        // i.e. print out end of game info
        gameFinish();
    }
}

void GameEngine::printGameInfo(int playerIndex) {
    std::cout << players[playerIndex]->getName() << ", it's your turn" << std::endl;
    for (Player *player : players) {
        std::cout << "Score for " << player->getName() << " is: " << player->getScore() << std::endl;
    }
    gameBoard.printBoard(std::cout) ;
    std::cout << "Your hand is " << std::endl;
    std::cout << players[playerIndex]->getHand()->getTiles() << std::endl;
}

void GameEngine::gameFinish(){
    // TODO: events upon game finish
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
