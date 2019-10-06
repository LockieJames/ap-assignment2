#include "Player.h"

Player::Player()
{
    hand = new LinkedList();
    score = 0;
}

Player::Player(std::string loadedName, int loadedScore, LinkedList* loadedHand){
    this->name = loadedName;
    this->score = loadedScore;
    this->hand = loadedHand;
}

Player::~Player()
{
//    clearTiles();
//    std::cout << "Got to start delete player" << std::endl;
    delete hand;
//    std::cout << "Got to end delete player" << std::endl;
}

std::string Player::getName()
{
    return name;
}

void Player::setName(std::string name)
{
    this->name = name;
}

int Player::getScore()
{
    return score;
}

void Player::addScore(int num)
{
    score += num;
}

LinkedList* Player::getHand()
{
    return hand;
}

int Player::getHandSize()
{
    return hand->size();
}
