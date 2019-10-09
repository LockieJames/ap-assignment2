
#ifndef SaveLoad_h
#define SaveLoad_h

#include <iostream>
#include <fstream>
#include <sstream>
#include "Board.h"
#include "TileBag.h"
#include "Menu.h"
#include "GameEngine.h"
#include "Player.h"

#define PLAYER_NAME_POS 0
#define PLAYER_SCORE_POS 1
#define PLAYER_HANDS_POS 2
#define MAX_COLS 26 / 2
#define MAX_ROWS 26

class GameEngine;

class SaveLoad {
    public:
        SaveLoad();
        ~SaveLoad();

        void saveGame(std::string fileName, int currentPlayer, std::vector<Player*>* players, Board* gameBoard, TileBag* tileBag);
        std::string loadGame(std::string filename, GameEngine* gameEngine);


    private:
        LinkedList* makeLinkedList(std::string tiles);
        void validateName(std::string name);
        int validateScore(std::string score);
        void validateTile(std::string tileString);
        std::vector<Tile*> parseBoardRow(std::string boardRowString, int columnCount);
        int getCurrPlayer(std::vector<Player*>* players, std::string currentPlayerName);
        void validateBorder(std::string line, int columnCount);
        int validateColCoords(std::string line, bool firstColCoordLine);
        void getNextLine(std::ifstream& file, std::string& line, bool lastLine);

};

#endif //SaveLoad_h
