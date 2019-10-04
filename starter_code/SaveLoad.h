//
//  SaveLoad.h
//
//
//  Created by Louis Baudinette on 4/10/2019
//

#ifndef SaveLoad_h
#define SaveLoad_h

// #includes decide later
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

class GameEngine;

class SaveLoad {
    public:
        SaveLoad();
        ~SaveLoad();

        void saveGame(std::string fileName, int currentPlayer, std::vector<Player*>* players, Board* gameBoard, TileBag* tileBag);
        void loadGame(std::string filename, GameEngine* gameEngine);

    private:

        LinkedList* makeLinkedList(std::string tiles);
        void validateName(std::string name);
        int validateScore(std::string score);
        void validateTile(std::string tileString);
        std::vector<Tile*> parseBoardRow(std::string boardRowString);
        int getCurrPlayer(std::vector<Player*>* players, std::string currentPlayerName);

};

#endif /* SaveLoad_h */
