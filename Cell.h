// Cell.h
#ifndef CELL_H
#define CELL_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <random>

class Cell {
public:
    Cell(int xLoc, int yLoc, int height, int width);

    int getLocationX() const;
    int getLocationY() const;
    bool isAlive() const;

private:
    int LocationX;
    int LocationY;
    bool Alive;

    sf::RectangleShape shape;
};

#endif // CELL_H
