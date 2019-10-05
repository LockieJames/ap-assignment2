//
//  SaveLoad.cpp
//
//
//  Created by Louis Baudinette on 4/10/2019
//

#include "SaveLoad.h"

SaveLoad::SaveLoad(){

}

SaveLoad::~SaveLoad(){

}

void SaveLoad::saveGame(std::string fileName, int currentPlayer, std::vector<Player*>* players, Board* gameBoard, TileBag* tileBag){
    // set up file to be written to
    std::ofstream file;
    fileName.append(".txt");
    file.open(fileName);

    // get player names, scores, hands and write to file
    for (auto i : *players){
        file << i->getName() << std::endl;
        file << i->getScore() << std::endl;
        file << i->getHand()->getTiles() << std::endl;

    }

    // get game board and write to file
    gameBoard->printBoard(file);

    // get all tiles in tilebag and write to file
    file << tileBag->getTileBag()->getTiles() << std::endl;

    // get name of current player and write to file
    file << players->at(currentPlayer)->getName();

    // close file stream
    file.close();
}

void SaveLoad::loadGame(std::string fileName, GameEngine* gameEngine){

    // declare objects for data to be loaded
    std::vector<Player*>* playersVector = new std::vector<Player*>;
    std::vector<std::vector<Tile*>> grid;
    LinkedList* tileBag = nullptr;
    std::string playerName;
    int currPlayerIndex = -1;
    
    // read from file. the end of each step gets the first line of the next in
    // order to check if current step is finished
    try {

        // open file
        std::ifstream file;
        file.open(fileName);
        std::string line;
        while (!file.eof()){

            // loading player data
            bool players = true;
            std::string name;
            int score = -1;
            LinkedList* hand = nullptr;

            while(players){
                // iterate through cycles of 3 lines and get relevant data from each 
                bool makeNewPlayer = false;
                for (int i = PLAYER_NAME_POS; i <= PLAYER_HANDS_POS; i++){
                    getline(file, line);

                    if (line[0] != ' '){
                        if (i == PLAYER_NAME_POS){
                            validateName(line);
                            name = line;

                        } else if (i == PLAYER_SCORE_POS){
                            score = validateScore(line);

                        } else if (i == PLAYER_HANDS_POS){
                            hand = makeLinkedList(line);
                            makeNewPlayer = true;
                        }

                    } else if (line[0] == ' ' && i == PLAYER_NAME_POS){
                        // if this line is the game board, and the program is
                        // checking for player's name, stop checking for player data
                        i = 3;
                        players = false;
                    } else if (line[0] == ' ' && i != PLAYER_NAME_POS){
                        // if this line is the game board, and the program is
                        // not checking for the player's name (i.e. save file
                        // is structured wrong),
                        // throw InvalidSaveFile exception
                    }
                }

                // add player new player based on loaded data to player vector
                if (makeNewPlayer){
                    playersVector->push_back(new Player(name, score, hand));
                }

            }
            
            // loading board data
            bool board = true;
            // define regex expressions to identify different lines of board
            std::regex boardRows = std::regex("[A-Z]\\s");
            std::regex tiles = std::regex("[ROYGBP][123456]");  
            while (board){
                // if line is a board row, add it to vector of rowsx
                if (std::regex_match(line.substr(0, 2), boardRows)){
                    grid.push_back(parseBoardRow(line));

                }
                getline(file, line);

                // if line is no longer a board
                if (std::regex_match(line.substr(0, 2), tiles)){
                    board = false;
                }
            }

            // load tiles in tile bag
            tileBag = makeLinkedList(line);

            // get and validate name of player whose turn it is
            getline(file, line);
            validateName(line);
            playerName = line;

            // get index of current player's name
            currPlayerIndex = getCurrPlayer(playersVector, playerName);
        }

        file.close();

    } catch (const std::ifstream::failure& e){
        std::cout << "Error opening/reading the file" << std::endl;
    }

    gameEngine->loadGameState(playersVector, grid, tileBag, currPlayerIndex);

}

LinkedList* SaveLoad::makeLinkedList(std::string tiles){
    std::vector<std::string> tileContainer;
    std::stringstream ss(tiles);
    std::string tile;
    while (std::getline(ss, tile, ',')){
        tileContainer.push_back(tile);
    }

    LinkedList* linkedList = new LinkedList();
    for (auto i : tileContainer){
        validateTile(i);
        std::cout << i[0] << std::endl;
        std::cout << i[1] << std::endl;
        linkedList->addFront(new Tile(i[0], i[1] - '0'));
    }

    return linkedList;
}

void SaveLoad::validateName(std::string name){
    std::regex validInput = std::regex("[A-Z]+");
    if (!std::regex_match(name, validInput)){
        //throw InvalidName exception
    }
}

int SaveLoad::validateScore(std::string score){
    if (!(score == std::to_string(std::stoi(score)))){
        // throw InvalidScore exception
    }
    return std::stoi(score);
}

void SaveLoad::validateTile(std::string tileString){
    std::regex validTile = std::regex("[ROYGBP][123456]");
    if (!(std::regex_match(tileString, validTile))){
        // throw InvalidTile exception
    }

}

std::vector<Tile*> SaveLoad::parseBoardRow(std::string boardRowString){
    int rowStart = boardRowString.find('|');
    std::vector<Tile*> boardRow;
    std::stringstream ss(boardRowString.substr(rowStart));
    std::string tile;
    int i = 0;
    while (std::getline(ss, tile, '|')){
        boardRow.resize(boardRow.size() + 1);

        tile.erase(remove(tile.begin(), tile.end(), ' '), tile.end());

        if (!tile.empty()){
            validateTile(tile);
            boardRow[i] = new Tile(tile[0], tile[1]);

        }

        i++;
    }

    return boardRow;
}

int SaveLoad::getCurrPlayer(std::vector<Player*>* players, std::string currentPlayerName){
    int currentPlayerIndex = -1;
    for (int i = 0; i < (int) players->size(); i++){
        if (players->at(i)->getName() == currentPlayerName){
            currentPlayerIndex = i;
        }
    }

    if (currentPlayerIndex == -1){
        // throw InvalidPlayerName exception
    }

    return currentPlayerIndex;
}
