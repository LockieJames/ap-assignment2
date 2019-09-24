#include "Player.h"

Player::Player()
{
    hand = new LinkedList();
    score = 0;
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
