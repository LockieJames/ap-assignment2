#include "GameEngine.h"
#include "Highscore.h"

#include <iostream>
#include <string>

#define EXIT_SUCCESS    0

int main(void)
{
    GameEngine* gameEngine = new GameEngine();
    gameEngine->mainMenu();
    delete gameEngine;
    
    return EXIT_SUCCESS;
}
