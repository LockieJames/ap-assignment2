#include "GameEngine.h"

#include <iostream>
#include <string>

#define EXIT_SUCCESS    0
#define NUM_STUDENTS    4

int main(void)
{
    int numPlayers = 2;
    GameEngine gameEngine(numPlayers);
    gameEngine.mainMenu();

    std::cout << "GOT HERE" << std::endl;
    
    return EXIT_SUCCESS;
}
