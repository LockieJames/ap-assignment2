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

/*
 *
 */
void SaveLoad::saveGame(std::string fileName, int currentPlayer, std::vector<Player*>* players, Board* gameBoard, TileBag* tileBag){
    // set up file to be written to
    std::ofstream file;
    fileName.append(".txt");
    file.open(fileName);

    // get player names, scores, hands and write to file
    for (auto i : *players){
        file << i->getName() << std::endl;
        file << i->getScore() << std::endl;
        file << i->getHand()->getTiles(false, false) << std::endl;

    }

    // get game board and write to file
    gameBoard->printBoard(file, false);

    // get all tiles in tilebag and write to file
    file << tileBag->getTileBag()->getTiles(false, false) << std::endl;

    // get name of current player and write to file
    file << players->at(currentPlayer)->getName();

    // close file stream
    file.close();
}

/*
 *
 */
std::string SaveLoad::loadGame(std::string fileName, GameEngine* gameEngine){

    std::string errorMsg;

    // declare objects for data to be loaded
    std::vector<Player*>* playersVector = new std::vector<Player*>;
    std::vector<std::vector<Tile*>> grid;
    LinkedList* tileBag = nullptr;
    std::string playerName;
    bool firstRowOffset = false;
    int currPlayerIndex = -1;

    // read from file. the end of each step gets the first line of the next in
    // order to check if current step is finished
    // open file
    std::ifstream file;
    try {

        file.open(fileName);
        if (!file){
            throw std::ifstream::failure("Error: file with inputted filename was not found");
        }

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
                        throw std::ifstream::failure("Error: invalid save file");
                    }
                }

                // add player new player based on loaded data to player vector
                if (makeNewPlayer){
                    playersVector->push_back(new Player(name, score, hand));
                }

            }

            // loading board data
            // validate upper column coord line
            int columnCount = validateColCoords(line, true);

            // validate upper board border line
            getline(file, line);
            validateBorder(line, columnCount);


            // validate and load tile placement data
            bool board = true;
            // define regex expressions to identify different lines of board
            std::regex boardRows = std::regex("[A-Z]\\s\\s");
            std::regex tiles = std::regex("[ROYGBP][123456]");
            bool firstRow = true;
            while (board){
                getline(file, line);

                // if line is a board row, add it to vector of rows
                if (std::regex_match(line.substr(0, 3), boardRows)){

                    // if first row of board, check if first row is offset
                    if (firstRow){
                        std::regex rowOffsetCheck = std::regex("A\\s\\s\\s\\s\\s|");
                        if (std::regex_match(line.substr(0, 7), rowOffsetCheck)){
                            // if true, load first row at offset
                            firstRowOffset = true;
                        }
                        firstRow = false;
                    }

                    grid.push_back(parseBoardRow(line, columnCount));

                } else {
                    // if line is no longer a board row
                    board = false;
                }
            }

            // check that loaded row count does not exceed max
            if (grid.size() > MAX_ROWS){
                throw std::ifstream::failure("Error: invalid save file - loaded board has more than the maximum number of rows");
            }

            // validate lower board border line
            validateBorder(line, columnCount);

            // validate lower column coord line
            getline(file, line);
            if (line.substr(0, 3) == "   "){
                if (columnCount != validateColCoords(line.substr(3), false)){
                    throw std::ifstream::failure("Error: invalid save file - loaded board has an inconsistent number of columns");
                }
            } else {
                throw std::ifstream::failure("Error: invalid save file - column coordinates bounding the board were incorrect");
            }

            // load tiles in tile bag
            getline(file, line);
            tileBag = makeLinkedList(line);

            // get and validate name of player whose turn it is
            getline(file, line);
            validateName(line);
            playerName = line;

            // get index of current player's name
            currPlayerIndex = getCurrPlayer(playersVector, playerName);

            file.close();
        }

    } catch (const std::ifstream::failure& e){
        errorMsg= e.what();
    }

    if (errorMsg.empty()){
        gameEngine->loadGameState(playersVector, grid, firstRowOffset, tileBag, currPlayerIndex);
    } else {

        // if error, delete memory on heap
        // delete players
        for (auto i : *playersVector){
            delete i;
        }
        delete playersVector;

        // delete tiles in board
        for (auto i : grid){
            for (auto j : i){
                delete j;
            }
        }

        delete tileBag;
    }

    return errorMsg;
}

/*
 *
 */
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
        linkedList->addEnd(new Tile(i[0], i[1] - '0'));
    }

    return linkedList;
}

/*
 *
 */
void SaveLoad::validateName(std::string name) noexcept(false){
    std::regex validInput = std::regex("[A-Z]+");
    if (!std::regex_match(name, validInput)){
        throw std::ifstream::failure("Error: invalid save file - loaded player name was invalid");
    }
}

/*
 *
 */
int SaveLoad::validateScore(std::string score) noexcept(false){
    if (!(score == std::to_string(std::stoi(score)))){
        throw std::ifstream::failure("Error: invalid save file - loaded player score was invalid");
    }
    return std::stoi(score);
}

/*
 *
 */
void SaveLoad::validateTile(std::string tileString) noexcept(false){
    std::regex validTile = std::regex("[ROYGBP][123456]");
    if (!(std::regex_match(tileString, validTile))){
        throw std::ifstream::failure("Error: invalid save file - loaded tile was invalid");
    }

}

std::vector<Tile*> SaveLoad::parseBoardRow(std::string boardRowString, int columnCount) noexcept(false){
    int rowStart = boardRowString.find('|');
    std::vector<Tile*> boardRow;
    std::stringstream ss(boardRowString.substr(rowStart + 1));
    std::string tile;
    int i = 0;
    while (std::getline(ss, tile, '|')){
        if (i > MAX_COLS){
            throw std::ifstream::failure("Error: invalid save file - loaded board has more than the maximum number of columns");
        }
        if (i > columnCount){
            throw std::ifstream::failure("Error: invalid save file - loaded board has an inconsistent number of columns");
        }
        boardRow.resize(boardRow.size() + 1);

        tile.erase(remove(tile.begin(), tile.end(), ' '), tile.end());

        if (!tile.empty()){
            validateTile(tile);
            boardRow[i] = new Tile(tile[0], tile[1] - '0');

        }

        i++;
    }

    return boardRow;
}

/*
 *
 */
int SaveLoad::getCurrPlayer(std::vector<Player*>* players, std::string currentPlayerName) noexcept(false){
    int currentPlayerIndex = -1;
    for (int i = 0; i < (int) players->size(); i++){
        if (players->at(i)->getName() == currentPlayerName){
            currentPlayerIndex = i;
        }
    }

    if (currentPlayerIndex == -1){
        throw std::ifstream::failure("Error: invalid save file - player whose turn it currently is does not exist in loaded players");
    }

    return currentPlayerIndex;
}

void SaveLoad::validateBorder(std::string line, int columnCount) noexcept(false){
    if (line.substr(0, 3) == "   "){
        line = line.substr(3);
        if (line != std::string(columnCount * 5, '-') + "----"){
            throw std::ifstream::failure("Error: invalid save file - number of column coords in board exceeds max");
        }
    } else {
        throw std::ifstream::failure("Error: invalid save file - board was not formatted correctly");
    }
}

int SaveLoad::validateColCoords(std::string line, bool firstColCoordLine){
    int columnCount = -2;
    if (!firstColCoordLine){
        columnCount = -1;
    }
    if (line.substr(0, 2) == "  "){
        line = line.substr(2);
        while (!line.empty()){
            columnCount += 2;
            std::string colCountString = std::to_string(columnCount);
            if (
                (colCountString.length() != 1
                    || line.substr(0, 5) != "    " + colCountString)
                && (colCountString.length() != 2
                    || line.substr(0, 5) != "   " + colCountString)){
                throw std::ifstream::failure("Error: invalid save file - column coordinates bounding the board were incorrect");
            }
            line = line.substr(5);
            if (firstColCoordLine){
                if ((columnCount / 2) + 1 > MAX_COLS){
                    throw std::ifstream::failure("Error: invalid save file - number of column coords in board exceeds max");
                }
            } else {
                if ((columnCount + 1) / 2 > MAX_COLS){
                    throw std::ifstream::failure("Error: invalid save file - number of column coords in board exceeds max");
                }
            }
        }
    } else {
        throw std::ifstream::failure("Error: invalid save file - column coordinates bounding the board were incorrect");
    }

    // get total column count by processing iterator used to validate column coords
    if (firstColCoordLine){
        columnCount = (columnCount / 2) + 1;
    } else {
        columnCount = (columnCount + 1) / 2;
    }

    return columnCount;
}
