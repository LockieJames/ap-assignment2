#include "Node.h"

Node::Node(Tile* tile, Node* next, Node* prev) :
    tile(tile),
    next(next),
    prev(prev)
{
}

Node::Node(Node& other) :
    tile(other.tile),
    next(other.next),
    prev(other.prev)
{
}

Node::~Node() {
    next = nullptr;
    prev = nullptr;
}

