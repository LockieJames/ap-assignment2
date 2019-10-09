#include "Highscore.h"

Highscore::Highscore()
{
    
}

Highscore::~Highscore()
{
    
}

// Gets the size of the amount of high scores in the vector
int Highscore::getSize()
{
    return highscores.size();
}

// Returns the highscore at i
int Highscore::getHighscore(int i)
{
    return highscores[i];
}

// Returns the name of the highscore at i
std::string Highscore::getHighscoreName(int i)
{
    return highscoreNames[i];
}

// Loads highscores from the text file into the vectors
void Highscore::loadHighscores(std::string filename)
{
    std::ifstream file;
    
    try
    {
        file.open(filename);
        
        if(!file)
        {
            throw std::ifstream::failure(
                "Error: file with inputted filename was not found"
            );
        }
        
        std::string line;
        std::string delim = " ";
        std::string token;
        int count = 0;
        bool forceStop = false;
        
        while(!file.eof() && !forceStop)
        {
            getline(file, line);
            
            token = line.substr(0, line.find(delim));
            line.erase(0, line.find(delim) + delim.length());
            highscoreNames.push_back(token);
            std::stringstream ss(line);
            
            int score;
            ss >> score;
            highscores.push_back(score);
            
            count++;
            
            if(count >= 10)
            {
                forceStop = true;
            }
        }
        
        file.close();
    }
    catch (const std::ifstream::failure& e)
    {
        //errorMsg= e.what();
    }
}

// Adds the name and score of the new highscorer to the list and deletes the 10th highscore
void Highscore::addHighscore(std::string name, int score)
{
    if(highscores.size() < 10 && score <= highscores[highscores.size() - 1])
    {
        highscores.push_back(score);
        highscoreNames.push_back(name);
    }
    else
    {
        bool isHighscore = false;
        std::vector<int>::iterator it = highscores.begin();
        std::vector<std::string>::iterator it2 = highscoreNames.begin();
        
        while(it < highscores.end() && it2 < highscoreNames.end())
        {
            if(score > *it && !isHighscore)
            {
                it = highscores.insert(it, score);
                it2 = highscoreNames.insert(it2, name);
                
                isHighscore = true;
            }
            
            it++;
            it2++;
        }
        
        if(isHighscore && highscores.size() > 10)
        {
            highscores.pop_back();
            highscoreNames.pop_back();
        }
    }
}

// Saves the new highscores from the vector to the file passed through
void Highscore::saveHighscore(std::string filename)
{
    std::ofstream file;
    file.open(filename);
    
    for(int i = 0; i < (int) highscores.size(); i++)
    {
        file << highscoreNames[i] << " " << highscores[i] << std::endl;
    }
    
    file.close();
}
