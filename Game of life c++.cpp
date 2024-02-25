#include <SFML/Graphics.hpp>

#include <SFML/Window.hpp>
#include <iostream>
#include <random>
#include <chrono>
using namespace sf;


class Cell {

private:


    bool alive;

public:

    int LocationX;
    int LocationY;

    RectangleShape shape;

    bool IsAlive()
    {
        return alive;
    }

    // Added this default constructor to stop the compiler from complaining, shouldn't be used.
    Cell() : LocationX(0), LocationY(0), alive(false), shape(Vector2f(99999, 99999)) {
        shape.setFillColor(Color::Red);
    }

    Cell(int xLoc, int yLoc, int cellSize)
    {
        LocationX = xLoc;
        LocationY = yLoc;

        shape = RectangleShape();

        shape.setPosition(xLoc, yLoc);

        shape.setSize(Vector2f(cellSize, cellSize));

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> randomNumber(1, 10);

        int randomNum = randomNumber(rng);

        if (randomNum <= 2)
        {
            SetLife(true);
        }
        else
        {
            SetLife(false);
        }

    }


   void SetLife(bool life)
   {
        if (life)
        {
            shape.setFillColor(Color::White);
            alive = true;
        }
        else
        {
            shape.setFillColor(Color::Black);
            alive = false;
        }
   }

};



RenderWindow window(VideoMode(1000, 600, Style::Resize), "Game window");

unsigned int screenWidth = VideoMode::getDesktopMode().width;
unsigned int screenHeight = VideoMode::getDesktopMode().height;

// Size of the cells in pixels
int cellSize;

// How many cells can fit into the screen.
int numCellsWide;
int numCellsHigh;

// Vector containing the cells shown on screen. 
std::vector<std::vector<Cell>> Cells;
// Cells being updated are updated inside here before being transfered to the Cells vector, to ensure the rules are played out correctly.
std::vector<std::vector<Cell>> CellsBuffer;

// Used to pause the simulation.
bool isPaused = false;

// This is used to prevent the program from updating the same cell repeatly when the left mouse button is held down.
sf::Vector2i lastUpdatedCellPos;

/// <summary>
/// Performs configuration for the window and the cells.
/// </summary>
void ConfigSetup()
{
    //window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(60);

    // Set the size of the cells.
    cellSize = 10;

    // Calculate how many cells can fit into the screen.
    numCellsWide = screenWidth / cellSize;
    numCellsHigh = screenHeight / cellSize;

    // Create a vector to store the cells, with its capacity set to the amount of cells being simulated.

    Cells = std::vector<std::vector<Cell>>(numCellsWide, std::vector<Cell>(numCellsHigh));

    CellsBuffer = std::vector<std::vector<Cell>>(numCellsWide, std::vector<Cell>(numCellsHigh));
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
            int xPos = i * cellSize;
            int yPos = j * cellSize;

            Cell newCell = Cell(xPos, yPos, cellSize);

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
    if (topLeftCell.IsAlive())
        aliveCount++;
    if (topCell.IsAlive())
        aliveCount++;
    if (topRightCell.IsAlive())
        aliveCount++;
    if (rightCell.IsAlive())
        aliveCount++;
    if (bottomRightCell.IsAlive())
        aliveCount++;
    if (bottomCell.IsAlive())
        aliveCount++;
    if (bottomLeftCell.IsAlive())
        aliveCount++;
    if (leftCell.IsAlive())
        aliveCount++;


    return aliveCount;
}


/// <summary>
/// Update the cell's life state and colour according to conway's game of life.
/// </summary>
void UpdateCell(int x, int y)
{
    // Retrieve the cell at the current position in the grid.
    // The cell is retreived from the buffer so changing the life state of this cell does not affect the others being updated this tick.
    Cell& cell = CellsBuffer[x][y];

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
    // If the pause is enabled stop the function.
    if (isPaused)
        return;

    // Copy each cell from Cells to the buffer.
    for (int xIndex = 0; xIndex < numCellsWide; xIndex++)
    {
        for (int yIndex = 0; yIndex < numCellsHigh; yIndex++)
        {
            CellsBuffer[xIndex][yIndex] = Cells[xIndex][yIndex];
        }
    }

    // Iterate through each cell in the 2d vector and update them.
    for (int xIndex = 0; xIndex < numCellsWide; xIndex++)
    {
        for (int yIndex = 0; yIndex < numCellsHigh; yIndex++)
        {

            UpdateCell(xIndex, yIndex);
            
        }
    }
    
    // After the cells in the buffer are updated tranfer it back to the main vector "Cells"
    for (int xIndex = 0; xIndex < numCellsWide; xIndex++)
    {
        for (int yIndex = 0; yIndex < numCellsHigh; yIndex++)
        {
            Cells[xIndex][yIndex] = CellsBuffer[xIndex][yIndex];
        }
    }
}

/// <summary>
/// Toggles whether or not the simulation is running.
/// </summary>
void Pause()
{
    // Might add more code here in the future
    isPaused = !isPaused;
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

/// <summary>
/// Used to to toggle the lifestate of cell located under the mouse.
/// </summary>
void toggleCellLifeStateUnderMouse()
{
    // Get the mouse position relative to the window.
   sf::Vector2i mousePos = sf::Mouse::getPosition(window);

   // Get the coordinates of the cell at the mouse location, this value is also its index in the vector.
   auto cellMousePos = mousePos / cellSize;

   // Prevent the same cell from being updating twice untill the mouse is moved to another cell.
   if (cellMousePos == lastUpdatedCellPos)
       return;

   lastUpdatedCellPos = cellMousePos;


   auto selectedCell = &Cells[cellMousePos.x][cellMousePos.y];
   auto selectedCellBuffer = &CellsBuffer[cellMousePos.x][cellMousePos.y];

   if (selectedCell == nullptr)
       return;

    // Toggle the cells lifestate.
    selectedCell->SetLife(!selectedCell->IsAlive());
    selectedCellBuffer->SetLife(!selectedCellBuffer->IsAlive());

    // Update the screen
    DrawShapes();
    window.display();
}

int main()
{
    ConfigSetup();
    GenerateCells();

    Event event;

    bool isSpacePressed = false;
    bool isLeftMouseButtonHeld = false;


    // Mouse position relative to the window.
    sf::Vector2i mousePos ;
    // Coordinates of the cell at the mouse location.
    sf::Vector2i cellMousePos;


    // Selected cell for user actions.
    // Using a pointer instead of a reference so the compiler doesn't complain about it not being initialized.
    Cell* selectedCell;
    // Same as above for this one holds a pointer to the selected cell in the buffer.
    Cell* selectedCellBuffer;


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

                    // This allows the user to click and hold to toggle cell life states.
                case Event::MouseButtonPressed:
                    isLeftMouseButtonHeld = true;
                    break;

                case Event::MouseButtonReleased:
                    isLeftMouseButtonHeld = false;
                    break;

                case Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Space)
                    {
                        Pause();
                    }
                    break;



                default:
                    break;

            }


        }
        

        // Only update the cells if 500 milliseconds have passed.
        auto currentTime = std::chrono::steady_clock::now();

        // Get the time difference in milliseconds.
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTick);
        
        // Allow the user to click on cells to toggle their lifestate.
        if (isLeftMouseButtonHeld)
            toggleCellLifeStateUnderMouse();

        if (duration >= std::chrono::milliseconds(100))
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

