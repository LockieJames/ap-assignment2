//
//  GameEngine.h
//
//
//  Created by Louis Baudinette on 24/09/2019
//

#ifndef GameEngine_h
#define GameEngine_h

// #includes decide later
#include <iostream>
#include <regex>
#include "Board.h"
#include "TileBag.h"
#include "Menu.h"
#include "Player.h"
#include "SaveLoad.h"
#include "Highscore.h"

#define MAX_HAND_SIZE 6

class SaveLoad;

class GameEngine {
    public:
        GameEngine();
        ~GameEngine();
        void loadGameState(std::vector<Player*>* loadedPlayers, std::vector<std::vector<Tile*>> loadedGameBoard, bool firstRowOffset, LinkedList* loadedTileBag, int currentPlayerIndex);
        void mainMenu();

    private:
        Board* gameBoard;
        TileBag* tileBag;
        Menu menu;
        std::vector<Player *>* players;
        int numPlayers;
        Highscore * hs;
        std::string highscoreFilename;

        void newGame();
        void gameLoop(int firstPlayerIndex);
        void gameFinish();
        bool gameEndCheck();
        bool placeTile(Player* player, Colour colour, Shape shape, char rowInput, int col);
        bool replaceTile(Player* player, Colour colour, Shape shape);
        void instantiateHand();
        void drawTile(Player* player);
        void saveGame(std::string fileName, int currentPlayer);
        bool loadGame();
        int correctRegex(int regex);

};

#endif /* GameEngine_h */
