#include <SFML/Graphics.hpp>

#include <SFML/Window.hpp>
#include <iostream>
using namespace sf;

int main()
{
    RenderWindow window(VideoMode(1000, 600, Style::Resize), "da window");

    unsigned int screenWidth = VideoMode::getDesktopMode().width;
    unsigned int screenHeight = VideoMode::getDesktopMode().height;

    window.setVerticalSyncEnabled(true);

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

        window.clear(Color::Black);
        
        RectangleShape shape(Vector2f(50.f, 50.f));

        RectangleShape shape2 = shape;
        
        shape.setFillColor(Color::White);
        shape2.setFillColor(Color::Black);

        // Get how many cells can fit into the screen.
        int numCellsWide = screenWidth / 50;
        int numCellsHigh = screenHeight / 50;

        std::vector<std::vector<sf::RectangleShape>> shapes(numCellsWide, std::vector<sf::RectangleShape>(numCellsHigh));


        for (int i = 0; i < numCellsWide; i++)
        {
            for (int j = 0; j < numCellsHigh; j++)
            {
                Text debugText;
                debugText.setString("i = " + std::to_string(i) + " j = " + std::to_string(j));

                debugText.setCharacterSize(10);
                debugText.setFont(font);
                debugText.setFillColor(Color::Red);

                debugText.setPosition(i * 50, j * 50);

                RectangleShape newCell = shape;

                // Even is white, odd is black.
                if ((i + j) % 2 == 0)
                    newCell.setFillColor(Color::White);
                else
                    newCell.setFillColor(Color::Black);

                shape.setPosition(Vector2f(i * 50, j * 50));

                // Draw it to the screen (might move this later).
                window.draw(debugText);
                window.draw(newCell);

                shapes[i][j] = newCell;
            }
        }





        window.draw(shape2);
        window.draw(shape);

        window.display();


    }

    return 0;
}