#include "GameEngine.h"
#include "Highscore.h"

#include <iostream>
#include <string>

#define EXIT_SUCCESS    0

int main(void)
{
    GameEngine* gameEngine = new GameEngine();
    gameEngine->mainMenu();
    std::cout << "deleting gameEngine" << std::endl;
    delete gameEngine;
    std::cout << "deleted gameEngine, exiting" << std::endl;
    
    return EXIT_SUCCESS;
}
