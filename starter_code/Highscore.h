
#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#define AMOUNT_OF_HIGHSCORES 10

class Highscore
{
    private:
        std::vector<int> highscores;
        std::vector<std::string> highscoreNames;
    
    public:
        Highscore();
        ~Highscore();
        int getSize();
        int getHighscore(int i);
        std::string getHighscoreName(int i);
        void loadHighscores(std::string filename);
        void addHighscore(std::string name, int score);
        void saveHighscore(std::string filename);
};


#endif
