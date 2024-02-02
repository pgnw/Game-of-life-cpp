#include <SFML/Graphics.hpp>

#include <SFML/Window.hpp>
#include <iostream>
#include <random>
using namespace sf;


class Cell {

public:

    int LocationX;
    int LocationY;

    bool Alive;

    RectangleShape shape;

    // Added this default constructor to stop the compiler from complaining, shouldn't be used.
    Cell() : LocationX(0), LocationY(0), Alive(false), shape(Vector2f(99999, 99999)) {
        shape.setFillColor(Color::Red);
    }

    Cell(int xLoc, int yLoc, int cellHeight, int cellWidth)
    {
        LocationX = xLoc;
        LocationY = yLoc;

        shape = RectangleShape();


        shape.setPosition(xLoc, yLoc);

        shape.setSize(Vector2f(cellHeight, cellWidth));

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> randomNumber(1, 10);

        int randomNum = randomNumber(rng);

        if (randomNum <= 5)
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



};



RenderWindow window(VideoMode(1000, 600, Style::Resize), "da window");

unsigned int screenWidth = VideoMode::getDesktopMode().width;
unsigned int screenHeight = VideoMode::getDesktopMode().height;

int cellHeight;
int cellWidth;

// Get how many cells can fit into the screen.
int numCellsWide = screenWidth / cellHeight;
int numCellsHigh = screenHeight / cellWidth;

std::vector<std::vector<Cell>> Cells(numCellsWide, std::vector<Cell>(numCellsHigh));

/// <summary>
/// Performs configuration for the window and the cells.
/// </summary>
void ConfigSetup()
{
    window.setVerticalSyncEnabled(true);

    cellHeight = 50;
    cellWidth = 50;
}

/// <summary>
/// Generates the cells for the simuation.
/// </summary>
void GenerateCells()
{

    window.clear(Color::Black);

    for (int i = 0; i < numCellsWide; i++)
    {
        for (int j = 0; j < numCellsHigh; j++)
        {

            // Get the positions to place the cells.
            int xPos = i * cellHeight;
            int yPos = j * cellWidth;



            Cell newCell = Cell(xPos, yPos, cellHeight, cellWidth);

            // Draw it to the screen (might move this later).
            window.draw(newCell.shape);

            Cells[i][j] = newCell;
        }
    }

    window.display();
}

void UpdateCells()
{
    // Iterate through each cell in the 2d vector and ...
    for (int xIndex = 0; xIndex < numCellsWide; xIndex++)
    {
        for (int yIndex = 0; yIndex < numCellsHigh; yIndex++)
        {
            Cell currentCell = Cells[xIndex][yIndex];



        }
    }
     
}

int main()
{
    ConfigSetup();
    GenerateCells();

    Event event;

    bool isMouseHeld = false;

    bool isSpacePressed;

    Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cout << ("error loading font");
    }

    while (window.isOpen())
    {
        // Check if the window has focus then, check if the space key was pressed.
        if (window.hasFocus())
            isSpacePressed = Keyboard::isKeyPressed(Keyboard::Space);

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case Event::Closed:
                    window.close();
                    break;

                case Event::MouseButtonPressed:
                    isMouseHeld = true;
                    break;

                case Event::MouseButtonReleased:
                    isMouseHeld = false;
                    break;


                default:
                    break;

            }


        }

        //window.display();


    }

    return 0;
}

