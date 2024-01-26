#include "Cell.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <random>
using namespace sf;

class Cell {

public:

	Cell(int xLoc, int yLoc, int height, int width )
	{
		LocationX = xLoc;
		LocationY = yLoc;

		RectangleShape shape(Vector2f(height, width));


		shape.setPosition(xLoc, yLoc);

		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> randomNumber(1, 10);

		if (randomNumber(rng) <= 2)
		{
			Alive = true;
			shape.setFillColor(Color::White);
		}
		else
		{
			Alive = false;
			shape.setFillColor(Color::Black);
		}
		
	}

	int LocationX;
	int LocationY;

	bool Alive;

private:
	RectangleShape shape;


};



