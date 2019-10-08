//
//  GameEngine.cpp
//
//
//  Created by Louis Baudinette on 24/09/2019
//

#include "GameEngine.h"

GameEngine::GameEngine(){
    // Tile objects made upon instantiating tilebag
    this->tileBag = new TileBag();
    this->gameBoard = new Board();
    this->menu = Menu();
    this->players = new std::vector<Player *>;
    
    this->hs = new Highscore();
    
    highscoreFilename = "Highscores.txt";
    hs->loadHighscores(highscoreFilename);
}

GameEngine::~GameEngine(){
    clear();
}

/*
 * Prints main menu using menu.menuOptions(), gets user's choice, and calls
 * appropriate methods
 */
void GameEngine::mainMenu() {
    menu.startMessage();

    bool validChoice;
    bool quit = false;
    bool check = false;
    std::string strInput;
    std::regex inputCheck = std::regex("[1-5]");
    int input = -1;

        while (!quit) {
            menu.menuOptions();
            validChoice = false;

            while (!validChoice){
                std::getline(std::cin, strInput);
                
                if(std::regex_match(strInput, inputCheck)){
                    input = std::stoi(strInput);
                    validChoice = true;
                } else if (std::cin.eof()){
                    quit = true;
                    validChoice = true;
                } else {
                    menu.invalidInput();
                    menu.printUserInputPrompt();
                }
            }

            if (input == 1) {
                int numOfPlayers = newNumPlayers();
                if (numOfPlayers != 0) {
                    check = newGame(numOfPlayers);
                }
                if (check != true) {
                    instantiateHand();
                    gameLoop(0);
                }
                quit = true;
            } else if (input == 2) {
                if (loadGame()){
                    quit = true;
                }
            } else if (input == 3) {
                menu.printHighscores(hs);
            } else if (input == 4) {
                menu.stuInfo();
            } else if (input == 5) {
                quit = true;
            }
        }

    // call quit method to print exit message as game is closing
    menu.quit();
}

/*
 *
 */
int GameEngine::newNumPlayers() {
    int input;
    bool validInt = false;
    
    menu.newGamePt1();
    menu.numOfPlayers();
    
    while (!validInt) {
        std::cin >> input;
        std::cin.get();
        
        if (std::cin.eof()) {
            validInt = true;
            input = 0;
        }
        else if (std::cin.fail() || !(input >= 1 && input <= 4)){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            menu.invalidInput();
            menu.printUserInputPrompt();
        } else {
            this->numPlayers = input;
            validInt = true;
        }
    }
    
    return input;
}

/*
 * Gets player name and sets name for each player, before starting main
 * game loop
 */
bool GameEngine::newGame(int numPlayers){
    bool validCheck = false;
    
    this->numPlayers = numPlayers;
    
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
            if (std::cin.eof()) {
                validName = true;
                validCheck = true;
                i = players->size();
            } else if (std::regex_match(name, validInput)){
                players->at(i)->setName(name);
                validName = true;
            } else {
                menu.invalidInput();
                menu.printUserInputPrompt();
            }
        }
    }
    return validCheck;
}

/*
 * Main game loop, beings on turn of player passed to method
 */
void GameEngine::gameLoop(int firstPlayerIndex){
    bool gameFinished = false;
    bool gameQuit = false;
    bool firstTurn = true;
    
    menu.newGamePt2();

    while (!gameFinished) {
        for (int i = 0; i < (int) players->size(); i++) {

            if (firstTurn) {
                i = firstPlayerIndex;
                firstTurn = false;
            }

            if (!gameQuit) {

                // have menu print the current game state
                this->menu.printGameInfo(players, i, gameBoard);

                bool turnComplete = false;
                while (!turnComplete) {

                    // TODO: Ask for input in menu and pass string userInput
                    std::string userInput;
                    std::smatch field;
                    menu.printUserInputPrompt();
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

/*
 *
 */
void GameEngine::gameFinish()
{
    // checks if each player has broken a highscore or not
    for(int i = 0; i < (int) players->size(); i++)
    {
        Player* currPlayer = players->at(i);
        hs->addHighscore(currPlayer->getName(), currPlayer->getScore());
    }
    
    // saves the highscores into the file.
    hs->saveHighscore(highscoreFilename);
    
    menu.gameFinish(players);
}

/*
 *
 */
bool GameEngine::placeTile(Player* player, Colour colour, Shape shape, char rowInput, int col) {
    // TODO: add appropriate menu callbacks to print info to console if need be
    int newShape = correctRegex(shape);

    bool placed = false;
    LinkedList* playerHand = player->getHand();
    
    Tile* tile = playerHand->removeTile(colour, newShape);

    if (tile != nullptr) {
        int score = gameBoard->placeTile(*tile, rowInput, col);
        if (score != 0 && score > 0) {
            player->addScore(score);
            drawTile(player);
            placed = true;
        } else {
            playerHand->addFront(tile);
        }
    }

    return placed;
}

/*
 *
 */
void GameEngine::drawTile(Player* player) {
    std::string errMsg;
    LinkedList* playerHand = player->getHand();
    LinkedList* showBag = tileBag->getTileBag();
    
    if (tileBag->size() != 0) {
        try {
            playerHand->addEnd(tileBag->getFront());
            showBag->deleteFront();
        } catch (const std::runtime_error& run) {
            errMsg = run.what();
            menu.printString(errMsg);
        } catch (const std::out_of_range& range) {
            errMsg = range.what();
            menu.printString(errMsg);
        }
        
    }
}

/*
 *
 */
bool GameEngine::replaceTile(Player* player, Colour colour, Shape shape){
    std::string errMsg;
    bool placed = false;
    int newShape = correctRegex(shape);
    LinkedList* playerHand = player->getHand();
    LinkedList* showBag = tileBag->getTileBag();
    Tile* frontTile = nullptr;
    Tile* tile = nullptr;
    Colour nColour;
    Shape nShape;
    
    try {
        Tile* frontTile = tileBag->getFront();
        
        // draw a tile from bag
        nColour = frontTile->getColour();
        nShape = frontTile->getShape();
        
        // remove a tile from players  hand
        tile = playerHand->removeTile(colour, newShape);
        
    } catch (const std::runtime_error& run) {
        errMsg = run.what();
        menu.printString(errMsg);
    } catch (const std::out_of_range& range) {
        errMsg = range.what();
        menu.printString(errMsg);
    }

    if (tile != nullptr) {
        if (frontTile != nullptr) {
            Tile* nTile = new Tile(nColour, nShape);

            playerHand->addEnd(nTile);

            // add player's tile to the bag
            showBag->addEnd(tile);

            // delete drawn node from tile bag
            showBag->deleteFront();

            placed = true;
        }
    }
    return placed;
}

/*
 *
 */
void GameEngine::instantiateHand() {
    std::string errMsg;
    LinkedList* showBag = tileBag->getTileBag();
    
    for (auto player : *players) {
        LinkedList* playerHand = player->getHand();
        
        // Starts loop to ensure each player gets six tiles
        for (int j = 0; j < MAX_HAND_SIZE; j++) {
            try {
                Tile* frontTile = tileBag->getFront();
                
                Colour nColour = frontTile->getColour();
                Shape nShape = frontTile->getShape();
                
                Tile* tile = new Tile(nColour, nShape);
                
                playerHand->addEnd(tile);
                showBag->deleteFront();
            } catch (const std::runtime_error& run) {
                errMsg = run.what();
                menu.printString(errMsg);
            } catch (const std::out_of_range& range) {
                errMsg = range.what();
                menu.printString(errMsg);
            }
        }
    }
}

/*
 *
 */
void GameEngine::saveGame(std::string fileName, int currentPlayer) {
    SaveLoad saveLoader;
    saveLoader.saveGame(fileName, currentPlayer, players, gameBoard, tileBag);

    // print that game was saved successfully
    menu.gameSaved();
}

/*
 *
 */
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
    if (std::cin.eof()) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.clear();
    }
    if (!errorMsg.empty()) {
        menu.printString(errorMsg);
    } else {
        loadSuccess = true;
    }

    return loadSuccess;
}

/*
 *
 */
void GameEngine::loadGameState(std::vector<Player*>* loadedPlayers, std::vector<std::vector<Tile*>> loadedGameBoard, bool firstRowOffset, LinkedList* loadedTileBag, int currentPlayerIndex){

    // delete existing objects on heap 
    clear();

    // load new objects
    this->players = loadedPlayers;
    this->gameBoard = new Board(loadedGameBoard, firstRowOffset);
    this->tileBag = new TileBag(loadedTileBag);

    // proceed into main game loop starting with the player whose turn it currently is
    gameLoop(currentPlayerIndex);
}

/*
 * Function is created to ensure the regex's numeral is converted from char to int
 * to be used in other functions.
 */
int GameEngine::correctRegex(int regex) {
    char zero = '0';
    int change = regex - zero;

    return change;
}

/*
 *
 */
void GameEngine::clear(){
    delete tileBag;
    delete gameBoard;
    for (int i = 0; i < (int) players->size(); i++){
        delete (*players)[i];
    }
    delete players;
}
