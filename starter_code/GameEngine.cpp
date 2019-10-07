//
//  GameEngine.cpp
//
//
//  Created by Louis Baudinette on 24/09/2019
//

#include "GameEngine.h"

GameEngine::GameEngine(){
    // Tilebag made upon instantiating it
    this->tileBag = new TileBag();
    this->gameBoard = new Board();
    this->menu = Menu();
    this->players = new std::vector<Player *>;
    
    this->hs = new Highscore();
    
    highscoreFilename = "Highscores.txt";
    hs->loadHighscores(highscoreFilename);
    
//    this->numPlayers = numPlayers;
//
//    for (int i = 0; i < numPlayers; i++) {
//        Player* player = new Player();
//        this->players->push_back(player);
//    }
}

GameEngine::~GameEngine(){
    std::cout << "deleting tilebag" << std::endl;
    delete tileBag;
    std::cout << "deleting gameBoard" << std::endl;
    delete gameBoard;
    std::cout << "deleting individual players" << std::endl;
    for (int i = 0; i < (int) players->size(); i++){
        delete (*players)[i];
    }
    std::cout << "deleting players vector" << std::endl;
    delete players;
    std::cout << "deletion finished" << std::endl;
}

// prints main menu using menu.menuOptions(), gets user's choice, and calls appropriate methods
void GameEngine::mainMenu() {
    menu.startMessage();

    bool validChoice;
    bool quit = false;
    std::string strInput;
    int input;

        while (!quit) {
            menu.menuOptions();
            validChoice = false;

            while (!validChoice){
                std::cin >> strInput;
                std::cin.get();
                
                if(isdigit(strInput[0]) && strInput.size() == 1)
                {
                    std::stringstream ss(strInput);
                    ss >> input;
                }
                
                if (std::cin.eof()) {
                    quit = true;
                    validChoice = true;
                } else if (std::cin.fail() || !(input >= 1 && input <= 5)){
                    std::cin.clear();
                    menu.invalidInput();
                    std::cout << "> ";
                } else {
                    validChoice = true;
                }
            }

            if (input == 1) {
                newGame();
                quit = true;
            } else if (input == 2) {
                if (loadGame()){
                    quit = true;
                }
            } else if (input == 3) {
                menu.printHighscores(hs);
            } else if (input == 4) {
                menu.stuInfo();
            }
            else if (input == 5) {
                quit = true;
            }
        }

    // call quit method to print exit message as game is closing
    menu.quit();
}

// gets player names and makes tilebag, then starts main game loop
void GameEngine::newGame(){
    int input;
    bool validInt = false;
    
    menu.newGamePt1();
    menu.numOfPlayers();
    
    while (!validInt) {
        std::cin >> input;
        std::cin.get();
        
        if (std::cin.fail() || !(input >= 1 && input <= 4)){
            std::cin.clear();
            menu.invalidInput();
            std::cout << "> ";
        } else {
            this->numPlayers = input;
            validInt = true;
        }
    }
    
    // Instantiate players
    for (int i = 0; i < numPlayers; i++) {
        Player* player = new Player();
        this->players->push_back(player);
    }
    
    for (int i = 0; i < (int) players->size(); i++) {
        menu.newGameNames(i + 1);
        bool validName = false;
        std::string name;
        std::regex validInput = std::regex("[A-Z]+");
        while (!validName){
            std::getline(std::cin, name);
            if (std::regex_match(name, validInput)){
                players->at(i)->setName(name);
                validName = true;
            } else {
                menu.invalidInput();
                std::cout << "> ";
            }
        }
    }
    menu.newGamePt2();
    
    // Instantiate players' hands
    instantiateHand();

    gameLoop(0);
}

// main game loop, begins on turn of player passed to method
void GameEngine::gameLoop(int firstPlayerIndex){
    bool gameFinished = false;
    bool gameQuit = false;
    bool firstTurn = true;

    while (!gameFinished) {
        for (int i = 0; i < (int) players->size(); i++) {

            if (firstTurn){
                i = firstPlayerIndex;
                firstTurn = false;
            }

            if (!gameQuit){

                // have menu print the current game state
                this->menu.printGameInfo(players, i, gameBoard);

                bool turnComplete = false;
                while (!turnComplete) {

                    // TODO: Ask for input in menu and pass string userInput
                    std::string userInput;
                    std::smatch field;
                    std::cout << "> ";
                    std::getline(std::cin, userInput);

                    if (std::regex_match(userInput, field, std::regex(R"(place\s[ROYGBP][1-6]\sat\s[A-Z][0-9]+)"))) {

                        // place tile
                        turnComplete = placeTile(players->at(i), field[0].str()[6], field[0].str()[7], field[0].str()[12], std::stoi(field[0].str().substr(13)));

                        // check if game is finished
                        if (players->at(i)->getHand()->size() == 0) {
                            gameFinished = true;
                        }

                        if (!turnComplete) {
                            menu.invalidPlay();
                        }

                    } else if (std::regex_match(userInput, field, std::regex(R"(replace\s[ROYGBP][1-6])"))) {
                        // replace tile
                        turnComplete = replaceTile(players->at(i), field[0].str()[8], field[0].str()[9]);

                    }  else if (std::regex_match(userInput, field, std::regex(R"(save\s[a-zA-Z0-9]+)"))) {
                        // save game
                        saveGame(field[0].str().substr(5), i);

                    } else if (userInput == "help") {
                        menu.getHelp();

                    } else if (userInput == "quit" || std::cin.eof()) {
                        // quit game
                        turnComplete = true;
                        gameQuit = true;
                        gameFinished = true;
                    } else if (userInput == "tilebag") {
                        // For debugging
                        tileBag->showBag();
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

void GameEngine::gameFinish()
{
    // checks if each player has broken a highscore or not
    for(int i = 0; i < players->size(); i++)
    {
        hs->addHighscore(players->at(i)->getName(), players->at(i)->getScore());
    }
    
    // saves the highscores into the file.
    hs->saveHighscore(highscoreFilename);
    
    menu.gameFinish(players);
}

bool GameEngine::placeTile(Player* player, Colour colour, Shape shape, char rowInput, int col) {
    // TODO: add appropriate menu callbacks to print info to console if need be
    int newShape = correctRegex(shape);

    bool placed = false;
    Tile* tile = player->getHand()->removeTile(colour, newShape);
    
//    std::cout << "Got to place tile" << std::endl;
    if (tile != nullptr) {
        int score = gameBoard->placeTile(*tile, rowInput, col);
        if (score != 0 && score != -20) {
            player->addScore(score);
            drawTile(player);
            placed = true;
        } else {
            player->getHand()->addFront(tile);
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

    return placed;
}

void GameEngine::drawTile(Player* player) {
    if (tileBag->size() != 0) {
        player->getHand()->addEnd(tileBag->getFront());

        tileBag->getTileBag()->deleteFront();
    }
}

bool GameEngine::replaceTile(Player* player, Colour colour, Shape shape){
    // TODO: add appropriate menu callbacks to print info to console if need be
    bool placed = false;

    int newShape = correctRegex(shape);

    // remove a tile from players  hand
    Tile* tile = player->getHand()->removeTile(colour, newShape);

    if (tile != nullptr) {
        // draw a tile from bag
        Colour nColour = tileBag->getFront()->getColour();
        Shape nShape = tileBag->getFront()->getShape();

        Tile* nTile = new Tile(nColour, nShape);

        player->getHand()->addEnd(nTile);

        // add player's tile to the bag
        tileBag->getTileBag()->addEnd(tile);

        // delete drawn node from tile bag
        tileBag->getTileBag()->deleteFront();

        placed = true;
    }

    return placed;
}

void GameEngine::instantiateHand() {
    for (auto player : *players) {
        for (int j = 0; j < MAX_HAND_SIZE; j++) {
            
            Colour nColour = tileBag->getFront()->getColour();
            Shape nShape = tileBag->getFront()->getShape();
            
            Tile* tile = new Tile(nColour, nShape);
            
            player->getHand()->addEnd(tile);
            tileBag->getTileBag()->deleteFront();
        }
    }
}

void GameEngine::saveGame(std::string fileName, int currentPlayer) {
    SaveLoad saveLoader;
    saveLoader.saveGame(fileName, currentPlayer, players, gameBoard, tileBag);

    // print that game was saved successfully
    menu.gameSaved();
}

bool GameEngine::loadGame() {
    bool loadSuccess = false;

    // get filename to be loaded from user
    menu.loadGame();
    std::string fileName;
    std::getline(std::cin, fileName);

    // attempt to load game
    SaveLoad saveLoader = SaveLoad();
    std::string errorMsg = saveLoader.loadGame(fileName, this);

    // if there was an error, print the error, otherwise indicate that the load was successful
    if (!errorMsg.empty()) {
        menu.printString(errorMsg);
    } else {
        loadSuccess = true;
    }

    return loadSuccess;
}

void GameEngine::loadGameState(std::vector<Player*>* loadedPlayers, std::vector<std::vector<Tile*>> loadedGameBoard, bool firstRowOffset, LinkedList* loadedTileBag, int currentPlayerIndex){

    // delete existing objects on heap 
    this->~GameEngine();

    // load new objects
    this->players = loadedPlayers;
    this->gameBoard = new Board(loadedGameBoard, firstRowOffset);
    this->tileBag = new TileBag(loadedTileBag);

    // proceed into main game loop starting with the player whose turn it currently is
    gameLoop(currentPlayerIndex);
}

/*
 * Function is created to ensure shape
 * is converted from char to int;
 */
int GameEngine::correctRegex(int regex) {
    char zero = '0';
    int change = regex - zero;

    return change;
}
