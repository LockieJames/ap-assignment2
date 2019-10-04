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
    return hand->size();
}

// TODO: The player placing a tile

// TODO: Replacing a tile in the players hand
void Player::replaceTile(Tile* tile)
{
    // Validate that tile is in hand
    if(hand->findNodeByTile(tile))
    {
        hand->deleteAtIndex(hand->getIndexByTile(tile));
    }
}
