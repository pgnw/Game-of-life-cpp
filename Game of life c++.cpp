#include <SFML/Graphics.hpp>

#include <SFML/Window.hpp>
#include <iostream>
#include <random>
#include <chrono>
using namespace sf;


class Cell {

private:




public:

    int LocationX;
    int LocationY;

    RectangleShape shape;
    bool Alive;

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

   void SetLife(bool life)
   {
        if (life)
        {
            shape.setFillColor(Color::White);
            Alive = true;
        }
        else
        {
            shape.setFillColor(Color::Black);
            Alive = false;
        }
   }

};



RenderWindow window(VideoMode(1000, 600, Style::Resize), "Game window");

unsigned int screenWidth = VideoMode::getDesktopMode().width;
unsigned int screenHeight = VideoMode::getDesktopMode().height;

// Size of the cells in pixels
int cellHeight;
int cellWidth;

// How many cells can fit into the screen.
int numCellsWide;
int numCellsHigh;

std::vector<std::vector<Cell>> Cells;

/// <summary>
/// Performs configuration for the window and the cells.
/// </summary>
void ConfigSetup()
{
    //window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(60);

    // Set the size of the cells.
    cellHeight = 10;
    cellWidth = 10;

    // Calculate how many cells can fit into the screen.
    numCellsWide = screenWidth / cellHeight;
    numCellsHigh = screenHeight / cellWidth;

    // Create a vector to store the cells, with its capacity set to the amount of cells being simulated.

    Cells = std::vector<std::vector<Cell>>(numCellsWide, std::vector<Cell>(numCellsHigh));
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
}

/// <summary>
/// Returns how many living cells surround the cell passed in.
/// </summary>
int  HowManyLivingNeighbors(int x, int y)
{
    int aliveCount = 0;

    // Determine the index of the cell to the left of the current one. If it goes beyond the left boundary, wrap around to the right side.
    int leftCellIndex = (x - 1 < 0) ? numCellsWide -1 : x - 1;

    // Same as the above for every other side.
    int rightCellIndex = (x + 1 > numCellsWide -1) ? 0 : x  +1;
    int upCellIndex = (y + 1 > numCellsHigh -1) ? 0 : y + 1;
    int bottomCellIndex = (y - 1 < 0) ? numCellsHigh -1 : y - 1;

    // Get the Cell classes from the indexs.
    Cell topLeftCell = Cells[leftCellIndex][upCellIndex];
    Cell topCell = Cells[x][upCellIndex];
    Cell topRightCell = Cells[rightCellIndex][upCellIndex];
    Cell rightCell = Cells[rightCellIndex][y];
    Cell bottomRightCell = Cells[rightCellIndex][bottomCellIndex];
    Cell bottomCell = Cells[x][bottomCellIndex];
    Cell bottomLeftCell = Cells[leftCellIndex][bottomCellIndex];
    Cell leftCell = Cells[leftCellIndex][y];

    // Check if each cell is alive and add it to the count.
    if (topLeftCell.Alive)
        aliveCount++;
    if (topCell.Alive)
        aliveCount++;
    if (topRightCell.Alive)
        aliveCount++;
    if (rightCell.Alive)
        aliveCount++;
    if (bottomRightCell.Alive)
        aliveCount++;
    if (bottomCell.Alive)
        aliveCount++;
    if (bottomLeftCell.Alive)
        aliveCount++;
    if (leftCell.Alive)
        aliveCount++;


    return aliveCount;
}


/// <summary>
/// Update the cell's life state and colour according to conway's game of life.
/// </summary>
void UpdateCell(int x, int y)
{
    // Retrieve the cell at the current position in the grid.
    Cell& cell = Cells[x][y];

    int aliveNeighbors = HowManyLivingNeighbors(x, y);

    if (aliveNeighbors > 3)
    {
        cell.SetLife(false);
    }
    else if (aliveNeighbors < 2)
    {
        cell.SetLife(false);
    }
    else if (aliveNeighbors == 3)
    {
        cell.SetLife(true);
    }
}
void UpdateCells()
{
    // Iterate through each cell in the 2d vector and ...
    for (int xIndex = 0; xIndex < numCellsWide; xIndex++)
    {
        for (int yIndex = 0; yIndex < numCellsHigh; yIndex++)
        {

            UpdateCell(xIndex, yIndex);
            
        }
    }
     
}

/// <summary>
/// Goes through each cell and draws it to the window.
/// </summary>
/// <returns></returns>
void DrawShapes()
{
    // Go through every cell and draw it to the window.
    for (int i = 0; i < numCellsWide; i++)
    {
        for (int j = 0; j < numCellsHigh; j++)
        {
            // Draw it to the screen
            window.draw(Cells[i][j].shape);

            
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

    auto lastTick = std::chrono::steady_clock::now();

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
        

        // Only update the cells if 500 milliseconds have passed.
        auto currentTime = std::chrono::steady_clock::now();

        // Get the time difference in milliseconds.
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTick);

        if (duration >= std::chrono::milliseconds(500))
        {
            window.clear(Color::Red);
            UpdateCells();
            DrawShapes();
            window.display();

            // Update the last tick to now.
            lastTick = std::chrono::steady_clock::now();
        }
        
        


    }

    return 0;
}

