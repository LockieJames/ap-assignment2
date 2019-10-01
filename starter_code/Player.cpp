#include "Player.h"

Player::Player()
{
    hand = new LinkedList();
    score = 0;
}

Player::~Player()
{
    delete hand;
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
    return hand.size();
}

// TODO: The player placing a tile
