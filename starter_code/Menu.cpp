//
//  Menu.cpp
//  
//
//  Created by Peter Bui on 22/9/19.
//

#include "Menu.h"

Menu::Menu(){
    
}

Menu::~Menu() {
    
}

void Menu::startMessage() {
    std::cout << "\n" << "Welcome to Qwirkle!" << std::endl;
    std::cout << "-------------------" << std::endl;
}

// Input must be an int
void Menu::menuOptions() {
    int input = 0;
    bool check = false;
    
    // Menu "crashes" if input is not an integer
    while (check == false) {
        std::cout << "Menu" << std::endl;
        std::cout << "----" << std::endl;
        std::cout << "1. New Game" << std::endl;
        std::cout << "2. Load Game" << std::endl;
        std::cout << "3. Show Student Information" << std::endl;
        std::cout << "4. Quit" << std::endl << std::endl;
        
        std::cout << "> ";
        
        std::cin >> input;
        
        check = choiceMenu(input);
    }
}

// Names should only be in uppercase
void Menu::newGame() {
    std::string player1;
    std::string player2;
    
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Starting a New Game" << std::endl;
    std::cout << "Enter a name for player 1 (Uppercase Chars only)" << std::endl;
    std::cin >> player1;
    
    std::cout << "Enter a name for player 2 (Uppercase Chars only)" << std::endl;
    std::cin >> player2;
    
    std::cout << "Let's Play!" << std::endl;
    std::cout << "----------------------------------" << std::endl;
}

void Menu::loadGame() {
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Not yet implemented" << std::endl;
    std::cout << "----------------------------------" << std::endl;
}

void Menu::stuInfo() {
    std::cout << "----------------------------------" << std::endl;
    for (int i = 0; i < NUM_STUDENTS; i++) {
        std::cout << name[i] << std::endl;
        std::cout << stu_id[i] << std::endl;
        std::cout << email[i] << std::endl;
        if (i < NUM_STUDENTS - 1) {
            std::cout << std::endl;
        }
    }
    std::cout << "----------------------------------" << std::endl;
}

void Menu::quit() {
    std::cout << "Goodbye" << std::endl;
}

bool Menu::choiceMenu(int input) {
    bool check = false;
    
    if (input == 1) {
        newGame();
    } else if (input == 2) {
        loadGame();
    } else if (input == 3) {
        stuInfo();
    } else if (input == 4) {
        check = true;
        quit();
    } else {
        std::cout << "Input not accepted" << std::endl;
    }
    
    return check;
}
