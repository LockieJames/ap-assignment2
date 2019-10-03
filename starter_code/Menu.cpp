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
    std::cout << "Menu" << std::endl;
    std::cout << "----" << std::endl;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Show Student Information" << std::endl;
    std::cout << "4. Quit" << std::endl << std::endl;
    
    std::cout << "> ";
}

void Menu::newGamePt1() {    
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Starting a New Game" << std::endl;
}

void Menu::newGameNames(int playerNo) {
    std::cout << "Enter a name for player " << playerNo << " (uppercase characters only)" << std::endl << "> ";
}

void Menu::newGamePt2() {
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

//int Menu::returnInput() {
//    std::string input;
//    std::string place ("Place");
//    std::string replace ("Replace");
//    int out = 0;
//
//    std::cout << "> ";
//    std::cin >> input;
//
//    // place
//    if (input.find(place)) {
//        out = 1;
//    } else if (input.find(replace)) { // replace
//        out = 2;
//    }
//
//    return out;
//}
