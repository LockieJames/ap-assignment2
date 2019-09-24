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

#define NUM_STUDENTS    4

class Menu {
    public:
        Menu();
        ~Menu();
    
        void startMessage();
        void menuOptions();
        void newGame();
        void loadGame();
        void stuInfo();
        void quit();
        bool choiceMenu(int input);
    
    private:
    // Everyone change personal information
    // Hardcoded for the time being; can change into another method/class
    // Dummy values in place for testing
    std::string name[NUM_STUDENTS] = {"Louis Baudinette", "Sim Bilkova", "Lockie Boughton", "Peter Bui"};
    std::string stu_id[NUM_STUDENTS] = {"s3668025", "s3748189", "s1234567", "s3786794"};
    std::string email[NUM_STUDENTS] = {"s3668025@student.rmit.edu.au", "s3748189@student.rmit.edu.au",
        "s1234567@student.rmit.edu.au", "s3786794@student.rmit.edu.au"};
};

#endif /* Menu_h */
