// graphics.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include <SFML/Graphics.hpp>

// TODO: This is an example of a library function
void test_draw()
{
    // This is a placeholder for the main function.
    // You can add your SFML code here to create a window, draw shapes, etc.

    // std::cout << "hello world" << std::endl;

    sf::RenderWindow window(sf::VideoMode({ 800, 800 }), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    // define a line from (10,10) to (100,100)

	sf::RectangleShape rect(sf::Vector2f(100.f, 10.f));



	

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(shape);
		window.draw(rect);
        window.display();
    }
}

