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
    std::cout << "3. Highscores" << std::endl;
    std::cout << "4. Show Student Information" << std::endl;
    std::cout << "5. Quit" << std::endl << std::endl;

    std::cout << PROMPT;
}

void Menu::newGamePt1() {
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Starting a New Game" << std::endl;
}

void Menu::numOfPlayers() {
    std::cout << "Number of players(MAX 4): " << std::endl << PROMPT;
}

void Menu::newGameNames(int playerNo) {
    std::cout << "Enter a name for player " << playerNo << " (uppercase characters only)" << std::endl << PROMPT;
}

void Menu::newGamePt2() {
    std::cout << "Let's Play!" << std::endl;
    std::cout << "----------------------------------" << std::endl;
}

void Menu::loadGame() {
    std::cout << "Enter the filename from which to load the a game" << std::endl;
    std::cout << PROMPT;
}

void Menu::loadGameSuccess() {
    std::cout << "Qwirkle game successfully loaded" << std::endl;
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

void Menu::printGameInfo(std::vector<Player *>* players, int currentPlayer, Board* gameBoard) {
    std::cout << players->at(currentPlayer)->getName() << ", it's your turn" << std::endl;
    for (auto i : *players) {
        std::cout << "Score for " << i->getName() << " is: " << i->getScore() << std::endl;
    }
    gameBoard->printBoard(std::cout, true);
    std::cout << "Your hand is " << std::endl;
    std::cout << "Input values: " << players->at(currentPlayer)->getHand()->getTiles(true, false) << std::endl;
    std::cout << "Visually are: " << players->at(currentPlayer)->getHand()->getTiles(true, true) << std::endl;
}



void Menu::gameFinish(std::vector<Player *>* players) {
    int finalScore = 0;
    int index = 0;
    
    std::cout << "Game Over" << std::endl;
    for (auto player : *players) {
        std::cout << "Score for " << player->getName() << ": " << player->getScore() << std::endl;
    }

    for (int i = 0; i < (int) players->size(); i++) {
        if (players->at(0)->getScore() > finalScore) {
            finalScore = players->at(0)->getScore();
            index = i;
        }
    }
    
    std::cout << "Player " << players->at(index)->getName() << "won!" << std::endl;
        
    quit();
}

void Menu::getHelp() {
    std::cout << "HELP!" << std::endl;
    std::cout << "Feeling a little stuck? Here are your options: " << std::endl;
    std::cout << "Write: " << std::endl;
    std::cout << "place (colour of the tile you want to place)(shape you want to place) at (selected row)(selected column) " << std::endl;
    std::cout << "\t This could for instance look like \033[38;5;126m\"place P4 at D5\"\033[0m" << std::endl;
    std::cout << "\t Be careful however, you are allowed to place tiles only next to one that already exists" << std::endl;
    std::cout << "\t and has the same shape or colour!" << std::endl;
    std::cout << "\t Additionally, you can place only the tiles that are in you hand!" << std::endl << std::endl;

    std::cout << "replace (colour of tile)(shape of tile)" << std::endl;
    std::cout << "\t This will replace the tile you choose and you will get a new one from the bag." << std::endl;
    std::cout << "\t Could look like \033[38;5;126m\"replace Y2\"\033[0m" << std::endl << std::endl;
    
    std::cout << "save (filename)" << std::endl;
    std::cout << "\t Will save the game into the file you provided." << std::endl << std::endl;

    std::cout << "quit" << std::endl;
    std::cout << "\t Will quit the game." << std::endl << std::endl;

    std::cout << "^D" << std::endl;
    std::cout << "\t Will quit the game as well." << std::endl << std::endl;

    std::cout << "help" << std::endl;
    std::cout << "\t Opens this." << std::endl << std::endl;
}

std::string Menu::printColour(Colour colour, std::ostream &destination) {
    std::string finalColour;

    if (typeid(destination).name() == typeid(std::cout).name()) {
        if (colour == RED) {
            finalColour = "\033[38;5;160mR\033[0m";
        } else if (colour == ORANGE) {
            finalColour = "\033[38;5;202mO\033[0m";
        } else if (colour == YELLOW) {
            finalColour = "\033[38;5;226mY\033[0m";
        } else if (colour == GREEN) {
            finalColour = "\033[38;5;40mG\033[0m";
        } else if (colour == BLUE) {
            finalColour = "\033[38;5;33mB\033[0m";
        } else if (colour == PURPLE) {
            finalColour = "\033[38;5;55mP\033[0m";
        }
    } else {
        finalColour = colour;
    }
    return finalColour;
}

std::string Menu::printShape(Shape shape, std::ostream &destination) {
    std::string finalColour;

    if (typeid(destination).name() == typeid(std::cout).name()) {
        if (shape == CIRCLE) {
            finalColour = "\u25CF";
        } else if (shape == STAR_4) {
            finalColour = "\u2726";
        } else if (shape == DIAMOND) {
            finalColour = "\u25C6";
        } else if (shape == SQUARE) {
            finalColour = "\u25A0";
        } else if (shape == STAR_6) {
            finalColour = "\u2736";
        } else if (shape == CLOVER) {
            finalColour = "\u2724";
        }
    } else {
        finalColour = shape;
    }
    return finalColour;
}

void Menu::invalidInput() {
    std::cout << "Invalid input!" << std::endl;
}

void Menu::invalidPlay() {
    std::cout << "Invalid play!" << std::endl;
}

void Menu::gameSaved(){
    std::cout << "Game saved successfully saved" << std::endl;
}

void Menu::printString(std::string strToPrint){
    std::cout << strToPrint << std::endl;
}

void Menu::printHighscores(Highscore * hs)
{
    std::cout << "Highscores!" << std::endl;
    std::cout << "----------------------------------" << std::endl;
    
    for(int i = 0; i < hs->getSize(); i++)
    {
        std::cout << i + 1 << ". " << hs->getHighscoreName(i) << "  " << hs->getHighscore(i) << std::endl;
    }
    
    std::cout << "----------------------------------" << std::endl;
}

void Menu::printUserInputPrompt(){
    std::cout << PROMPT;
}
