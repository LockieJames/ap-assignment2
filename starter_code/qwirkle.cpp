#include "LinkedList.h"

#include <iostream>
#include <string>

#define EXIT_SUCCESS    0
#define NUM_STUDENTS    4

// All classes may eventually be split into other classes
void startMessage();
void menu();
void newGame();
void loadGame();
void stuInfo();
void quit();
void loopMenu(int input);

int main(void)
{
//    LinkedList* list = new LinkedList();
    
    startMessage();
    menu();

    return EXIT_SUCCESS;
}

void startMessage() {
    std::cout << "Welcome to Qwirkle!" << std::endl;
    std::cout << "-------------------" << std::endl;
}

void menu() {
    int input = 0;
    
    std::cout << "Menu" << std::endl;
    std::cout << "----" << std::endl;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Show Student Information" << std::endl;
    std::cout << "4. Quit" << std::endl << std::endl;
    
    std::cin >> input;
    
    loopMenu(input);
}

void newGame() {
    std::string player1;
    std::string player2;
    
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Starting a New Game" << std::endl;
    std::cout << "Enter a name for player 1 (Uppercase Chars only)" << std::endl;
    std::cin >> player1;
    
    std::cout << "Enter a name for player 2 (Uppercase Chars only)" << std::endl;
    std::cin >> player2;

    // For testing purposes
    std::cout << "Player1: " << player1 << std::endl;
    std::cout << "Player2: " << player2 << std::endl;
    
    std::cout << "Let's Play!" << std::endl;
    std::cout << "----------------------------------" << std::endl;
    menu();
}

void loadGame() {
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Not yet implemented" << std::endl;
    std::cout << "----------------------------------" << std::endl;
    menu();
}

// Everybody change personal information
// Hardcoded for the time being; can change into another method/class
// Dummy values in place for testing
void stuInfo() {
    std::string name[] = {"Louis Baudinette", "Sim Bilkova", "Lockie Boughton", "Peter Bui"};
    std::string id[] = {"s1234567", "s3748189", "s1234567", "s3786794"};
    std::string email[] = {"s1234567@student.rmit.edu.au", "s3748189@student.rmit.edu.au",
                            "s1234567@student.rmit.edu.au", "s3786794@student.rmit.edu.au"};
    
    std::cout << "----------------------------------" << std::endl;
    for (int i = 0; i < NUM_STUDENTS; i++) {
        std::cout << name[i] << std::endl;
        std::cout << id[i] << std::endl;
        std::cout << email[i] << std::endl;
        if (i < NUM_STUDENTS - 1) {
            std::cout << std::endl;
        }
    }
    std::cout << "----------------------------------" << std::endl;
    
    menu();
}

void quit() {
    std::cout << "Goodbye" << std::endl;
}

void loopMenu(int input) {
    if (input == 1) {
        newGame();
    } else if (input == 2) {
        loadGame();
    } else if (input == 3) {
        stuInfo();
    } else if (input == 4) {
        quit();
    }
}
