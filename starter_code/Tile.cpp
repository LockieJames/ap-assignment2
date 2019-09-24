#include "Tile.h"

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

Colour Tile::getColour() {
    return colour;
}

Shape Tile::getShape() {
    return shape;
}
