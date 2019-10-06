#include "Tile.h"
#include <iostream>

Tile::Tile(Colour colour, Shape shape) :
    colour(colour),
    shape(shape)
{
}

Tile::Tile(Tile& other) :
colour(other.colour),
shape(other.shape)
{
}

Tile::~Tile() {
    // For debugging
//    std::cout << "Got to tile delete" << std::endl;
}

Colour Tile::getColour() {
    return colour;
}

Shape Tile::getShape() {
    return shape;
}
