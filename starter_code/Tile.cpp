#include "Tile.h"

Tile::Tile(Colour colour, Shape shape) {
    this->colour = colour;
    this->shape = shape;
}

Tile::Tile(Tile& other) {
    this->colour = other.colour;
    this->shape = other.shape;
}

Colour Tile::getColour() {
    return this->colour;
}

Shape Tile::getShape() {
    return this->shape;
}
