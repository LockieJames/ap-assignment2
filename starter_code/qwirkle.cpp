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
void choiceMenu(int input);

// Testing purposes
void testList(LinkedList* list);

int main(void)
{
    LinkedList* list = new LinkedList();
    
    testList(list);
    
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
    
    choiceMenu(input);
}

// Names should only be in uppercase
void newGame() {
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

void choiceMenu(int input) {
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

// Testing if methods within linkedlist work
void testList(LinkedList* list) {
    Tile* tile1 = new Tile('R', 1);
    Tile* tile2 = new Tile('G', 2);
    Tile* tile3 = new Tile('B', 3);
    Tile* tile4 = new Tile('Y', 4);

    list->addEnd(tile1);
    list->addEnd(tile2);
    list->addFront(tile3);
    list->addFront(tile4);

    for (int i = 0; i < list->size(); i++) {
        std::cout << list->get(i)->getColour();
        std::cout << list->get(i)->getShape() << std::endl;
    }
    
//    try {
//        list->deleteAtIndex(2);
//    }
//    catch (std::runtime_error e) {
//        std::cout << e.what() << std::endl;
//    } catch (std::out_of_range e) {
//        std::cout << e.what() << std::endl;
//    }
//
//    std::cout << "----" << std::endl;
//
//    for (int i = 0; i < list->size(); i++) {
//        std::cout << list->get(i)->getColour();
//        std::cout << list->get(i)->getShape() << std::endl;
//    }
}

