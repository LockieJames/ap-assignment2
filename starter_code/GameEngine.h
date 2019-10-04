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
#include "Board.h"
#include "TileBag.h"
#include "Menu.h"
#include "Player.h"

class GameEngine {
    public:
    GameEngine(int numPlayers);
    ~GameEngine();

    // todo: implement methods for game states and player turns
    void mainMenu();

    private:
    Board gameBoard;
    TileBag tileBag;
    Menu menu;
    std::vector<Player *> players;
    int numPlayers;

    void newGame();
    void gameLoop();
    void gameFinish();
    bool gameEndCheck();
    bool placeTile(Player player, Colour colour, Shape shape, char rowInput, int col);
    bool replaceTile(Player player, Colour colour, Shape shape);
    void instantiateHand();
    bool saveGame();
    void drawTile(Player player);
    bool saveGame();
    bool loadGame();

};

#endif /* GameEngine_h */
