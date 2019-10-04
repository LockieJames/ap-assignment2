//
//  GameEngine.cpp
//
//
//  Created by Louis Baudinette on 24/09/2019
//

#include <regex>
#include "GameEngine.h"

#define MAX_HAND_SIZE 6

GameEngine::GameEngine(int numPlayers){
    this->tileBag = TileBag();
    this->gameBoard = Board();
    this->menu = Menu();
    this->numPlayers = numPlayers;

    for (int i = 0; i < numPlayers; i++) {
        Player* player = new Player();
        this->players.push_back(player);
    }
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
            std::cin.get();
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

    // Instantiate players' hands
    instantiateHand();
    
    gameLoop();
}

// main game loop
void GameEngine::gameLoop(){
    bool gameFinished = false;
    bool gameQuit = false;

    while (!gameFinished) {
        for (int i = 0; i < (int) players.size(); i++) {

            if (!gameQuit){

                // have menu print the current game state
                menu.printGameInfo(players, i, gameBoard);

                bool turnComplete = false;
                while (!turnComplete) {

                    // TODO: Ask for input in menu and pass string userInput
                    std::string userInput;
                    std::smatch field;
                    std::cout << "> ";
                    std::getline(std::cin, userInput);

                    if (std::regex_match(userInput, field, std::regex(R"(place\s[ROYGBP][1-6]\sat\s[A-F][0-7])"))) {
                        // place tile
                        turnComplete = placeTile(*players.at(i), field[0].str()[6], field[0].str()[7],
                                                field[0].str()[12], field[0].str()[13]);

                        // check if game is finished
                        if (tileBag.size() == 0 && players.at(i)->getHand()->size() == 0) {
                            gameFinished = true;
                        }

                    } else if (std::regex_match(userInput, field, std::regex(R"(replace\s[ROYGBP][1-6])"))) {
                        // replace tile
                        turnComplete = replaceTile(*players.at(i), field[0].str()[8], field[0].str()[9]);

                    }  else if (std::regex_match(userInput, field, std::regex(R"(save\s[a-zA-Z0-9]+)"))) {
                        // save game
                        saveGame(field[0].str().substr(5), i);

                    } else if (userInput == "quit") {
                        // quit game
                        turnComplete = true;
                        gameQuit = true;
                        gameFinished = true;

                    } else {
                        // else print that input is invalid
                        menu.invalidInput();

                    }

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

void GameEngine::instantiateHand() {
    for (int i = 0; i < numPlayers; i++) {
        for (int j = 0; j < MAX_HAND_SIZE; j++) {
            Tile* tile = tileBag.getTileBag()->get(0);
            players.at(i)->getHand()->addEnd(tile);
            tileBag.getTileBag()->deleteFront();
        }
    }
}

void GameEngine::saveGame(std::string fileName, int currentPlayer) {
    std::ofstream file;
    fileName.append(".txt");
    file.open(fileName);

    // get player names, scores, hands and write to file
    for (auto i : players){
        std::cout << i->getName() << std::endl;
        file << i->getName() << std::endl;

        std::cout << i->getScore() << std::endl;
        file << i->getScore() << std::endl;

        std::cout << i->getHand()->getTiles() << std::endl;
        file << i->getHand()->getTiles() << std::endl;
    }

    // get game board and write to file
    gameBoard.printBoard(std::cout);
    gameBoard.printBoard(file);

    // get all tiles in tilebag and write to file
    file << tileBag.getTileBag()->getTiles() << std::endl;

    // get name of current player and write to file
    file << players.at(currentPlayer)->getName() << std::endl;

    file.close();

    menu.gameSaved();
}

void GameEngine::loadGame() {
}
