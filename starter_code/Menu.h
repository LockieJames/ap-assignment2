//
//  Menu.hpp
//
//
//  Created by Peter Bui on 22/9/19.
//

#ifndef Menu_h
#define Menu_h

// #includes decide later
#include <iostream>
#include <vector>
#include "Player.h"
#include "Board.h"

#define NUM_STUDENTS    4

class Menu {
    public:
        Menu();
        ~Menu();

        void startMessage();
        void menuOptions();
        void newGamePt1();
        void newGameNames(int playerNo);
        void newGamePt2();
        void loadGame();
        void stuInfo();
        void quit();
        void printGameInfo(std::vector<Player *> players, int currentPlayer, Board gameBoard);
        void gameFinish(std::vector<Player *> players);
        void invalidInput();
        void invalidPlay();
        void gameSaved();

    private:
    std::string name[NUM_STUDENTS] = {"Louis Baudinette", "Sim Bilkova", "Lachlan Boughton", "Peter Bui"};
    std::string stu_id[NUM_STUDENTS] = {"s3668025", "s3748189", "s3769219", "s3786794"};
    std::string email[NUM_STUDENTS] = {"s3668025@student.rmit.edu.au", "s3748189@student.rmit.edu.au",
        "s3769219@student.rmit.edu.au", "s3786794@student.rmit.edu.au"};
};

#endif /* Menu_h */
