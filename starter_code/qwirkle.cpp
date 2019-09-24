#include "LinkedList.h"
#include "Menu.h"
#include "TileBag.h"

#include <iostream>
#include <string>

#define EXIT_SUCCESS    0
#define NUM_STUDENTS    4

int main(void)
{
    TileBag* baggage = new TileBag();
    Menu* menu = new Menu();

    baggage->showBag();

    menu->startMessage();
    menu->menuOptions();
    
    return EXIT_SUCCESS;
}
