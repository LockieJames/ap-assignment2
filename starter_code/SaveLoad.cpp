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
 * Main saving method. Takes a filename and various data from the GameEngine,
 * and writes the data to the file with the given filename.
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
 * Main loading method. Takes a filename and a pointer to the current
 * GameEngine object, opens and loads data from the file with the given
 * filename, and passes the loaded data to the pointed-to GameEngine object.
 * Returns an error message; if the error message if empty, then there was no
 * error.
 */
std::string SaveLoad::loadGame(std::string fileName, GameEngine* gameEngine){

    std::string errorMsg;

    // declare objects for data to be loaded into
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
        // loading player data
        bool players = true;
        std::string name;
        int score = -1;
        LinkedList* hand = nullptr;

        while(players){
            // iterate through cycles of 3 lines and get relevant data from each
            bool makeNewPlayer = false;
            for (int i = PLAYER_NAME_POS; i <= PLAYER_HANDS_POS; i++){
                getNextLine(file, line, false);

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
                    throw std::ifstream::failure("Error: invalid save file - incorrectly formatted player data");
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
        getNextLine(file, line, false);
        validateBorder(line, columnCount);


        // validate and load tile placement data
        bool board = true;
        // define regex expressions to identify different lines of board
        std::regex boardRows = std::regex("[A-Z]\\s\\s");
        std::regex tiles = std::regex("[ROYGBP][123456]");
        bool firstRow = true;
        while (board){
            getNextLine(file, line, false);

            // if line is a board row, add it to vector of rows
            if (std::regex_match(line.substr(0, 3), boardRows)){

                // if first row of board, check if first row is offset
                if (firstRow){
                    if (line.substr(0, 7) == "A     |"){
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
        getNextLine(file, line, false);
        if (line.substr(0, 3) == "   "){
            if (columnCount != validateColCoords(line.substr(3), false)){
                throw std::ifstream::failure("Error: invalid save file - loaded board has an inconsistent number of columns");
            }
        } else {
            throw std::ifstream::failure("Error: invalid save file - column coordinates bounding the board were incorrect");
        }

        // load tiles in tile bag
        getNextLine(file, line, false);
        tileBag = makeLinkedList(line);

        // get and validate name of player whose turn it is
        getNextLine(file, line, false);
        validateName(line);
        playerName = line;

        // get index of current player's name
        currPlayerIndex = getCurrPlayer(playersVector, playerName);

        // close file
        getNextLine(file, line, true);

    } catch (const std::ifstream::failure& e){
        errorMsg= e.what();
    }

    // if there is no error, load game with loaded data
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
 * Helper method that take a string representing a row on the game board, and
 * processes the string into individual tiles, which are added to a LinkedList
 * and returned.
 */
LinkedList* SaveLoad::makeLinkedList(std::string tiles) {
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
 * Helper method that validates a given player name. Throws exception if name
 * does not match game's player name format.
 */
void SaveLoad::validateName(std::string name) {
    std::regex validInput = std::regex("[A-Z]+");
    if (!std::regex_match(name, validInput)){
        throw std::ifstream::failure("Error: invalid save file - loaded player name was invalid");
    }
}

/*
 * Helper method that validates a given score in string format and returns
 * the score as an integer. Throws an exception if score is not valid.
 */
int SaveLoad::validateScore(std::string score) {
    if (!(score == std::to_string(std::stoi(score)))){
        throw std::ifstream::failure("Error: invalid save file - loaded player score was invalid");
    }
    return std::stoi(score);
}

/*
 * Helper method that validates a given tile in string format, and throws
 * an exception if the tile doesn't match the game's format
 */
void SaveLoad::validateTile(std::string tileString) {
    std::regex validTile = std::regex("[ROYGBP][123456]");
    if (!(std::regex_match(tileString, validTile))){
        throw std::ifstream::failure("Error: invalid save file - loaded tile was invalid");
    }

}

/*
 * Helper method that takes a string representing a row of the game board,
 * validates the row, and returns the row as a vector of pointers to tile
 * objects.
 */
std::vector<Tile*> SaveLoad::parseBoardRow(std::string boardRowString, int columnCount) {
    int rowStart = boardRowString.find('|');
    std::vector<Tile*> boardRow;
    std::stringstream ss(boardRowString.substr(rowStart + 1));
    std::string tile;
    int i = 0;
    while (getline(ss, tile, '|')){

        // check that the number of tile spaces in the the board row
        // is not inconsistent or in excess of the max
        if (i > MAX_COLS){
            throw std::ifstream::failure("Error: invalid save file - loaded board has more than the maximum number of columns");
        }
        if (i > columnCount){
            throw std::ifstream::failure("Error: invalid save file - loaded board has an inconsistent number of columns");
        }

        // check that token representing a given tile space matches the
        // correct format
        if (tile != "    " && !std::regex_match(tile, std::regex(" [ROYGBP][123456] "))){
            throw std::ifstream::failure("Error: invalid save file - board was formatted incorrectly");
        }

        // resize the board row vector to accommodate the new tile
        boardRow.resize(boardRow.size() + 1);

        // remove whitespace from given tile space on board
        tile.erase(remove(tile.begin(), tile.end(), ' '), tile.end());

        // if tile space has a tile in it, add tile to board row vector 
        if (!tile.empty()){
            validateTile(tile);
            boardRow[i] = new Tile(tile[0], tile[1] - '0');

        }

        i++;
    }

    return boardRow;
}

/*
 * Helper method. Takes a vector of players and the name of the players whose
 * turn it currently is, and returns the index of that player.
 */
int SaveLoad::getCurrPlayer(std::vector<Player*>* players, std::string currentPlayerName) {
    int currentPlayerIndex = -1;

    // search for current player's name
    for (int i = 0; i < (int) players->size(); i++){
        if (players->at(i)->getName() == currentPlayerName){
            currentPlayerIndex = i;
        }
    }

    // throw exception if player with name wasn't found
    if (currentPlayerIndex == -1){
        throw std::ifstream::failure("Error: invalid save file - player whose turn it currently is does not exist in loaded players");
    }

    return currentPlayerIndex;
}

/*
 * Helper method that validates the bounding borders of the board based on 
 * the columnCount int passed to it.
 */
void SaveLoad::validateBorder(std::string line, int columnCount) {

    // check for initial padding, otherwise throw exception
    if (line.substr(0, 3) == "   "){

        // if padding is there, remove initial padding from line
        line = line.substr(3);

        // throw exception if border does not contain correct amount of
        // hyphens i.e. '-'
        if (line != std::string(columnCount * 5, '-') + "----"){
            throw std::ifstream::failure("Error: invalid save file - number of column coords in board exceeds max");
        }

    } else {
        throw std::ifstream::failure("Error: invalid save file - board was not formatted correctly");
    }
}

/*
 * Helper method that takes a string and validates that it matches up with the
 * correct format of a column coordinate line of the board. The bool
 * firstColCoordLine is used to swap from validating the upper column
 * coord line to the lower one.
 */
int SaveLoad::validateColCoords(std::string line, bool firstColCoordLine){
    // set columnCoord to the appropriate starting value based on whether
    // the line is the upper or lower col coord line
    int columnCount = -2;
    if (!firstColCoordLine){
        columnCount = -1;
    }
    
    // check for initial line padding, otherwise throw exception
    if (line.substr(0, 2) == "  "){

        // remove padding from line
        line = line.substr(2);

        // check whether each token (i.e. 4-5 spaces followed by an int) 
        // in the line is valid
        while (!line.empty()){

            // increment the columnCount
            // and get the column coord number for the current token
            columnCount += 2;
            std::string colCountString = std::to_string(columnCount);

            // throw exception if  the current token does not match
            // the correct format
            if (
                (colCountString.length() != 1
                    || line.substr(0, 5) != "    " + colCountString)
                && (colCountString.length() != 2
                    || line.substr(0, 5) != "   " + colCountString)){
                throw std::ifstream::failure("Error: invalid save file - column coordinates bounding the board were incorrect");
            }

            // if the token is in the correct format, remove the token from
            // the current line of tokens
            line = line.substr(5);

            // throw error if column count exceeds max
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

/*
 * Helper method that is responsible for getting new lines from the file.
 * Checks EOF against the bool lastLine to ensure that the file ends when
 * expected.
 */
void SaveLoad::getNextLine(std::ifstream& file, std::string& line, bool lastLine){
    if (!file.eof()){
        if (!lastLine){
            getline(file, line);
        } else {
            throw std::ifstream::failure("Error: invalid save file - file didn't end when expected");
        }
    } else if (file.eof()){
        if (!lastLine){
            throw std::ifstream::failure("Error: invalid save file - file ended earlier than expected");
        } else {
            file.close();
        }
    }
}