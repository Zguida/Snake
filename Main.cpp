#include <SFML/Graphics.hpp>
#include "snake.h"
#include <iostream>

void endGame(sf::RenderWindow& window) {
    sf::Font font("resources/game_over.ttf");
    sf::Text game_over = sf::Text(font, "GAME OVER", 100);
    game_over.setPosition(sf::Vector2f(((window.getSize().x / 2) - game_over.getLocalBounds().size.x / 2), ((window.getSize().y / 3) - game_over.getLocalBounds().size.y / 2)));
    window.draw(game_over);
}
int main()
{
    // Declare and create a new window
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML window");

    // Limit the framerate to 60 frames per second (this step is optional)
    window.setFramerateLimit(60);

    // The main loop - ends as soon as the window is closed
    snake the_snake = snake(sf::Vector2f(10, 10));


    bool key_buffer = NULL;
    bool isFrozen = false;
    sf::Vector2f current_direction;
    while (window.isOpen())
    {
        while (!isFrozen)
        {
            // Event processing
            while (const std::optional event = window.pollEvent())
            {
                // Request for closing the window
                if (event->is<sf::Event::Closed>())
                    window.close();
                if (event->is<sf::Event::KeyReleased>())
                    key_buffer = false;
            }
            //accepts keybaord input and updates movement
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && key_buffer == false && current_direction.y != 1) {
                the_snake.updateMovement(0, -1);
                current_direction = sf::Vector2f(0, -1);
                key_buffer = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && key_buffer == false && current_direction.x != 1) {
                the_snake.updateMovement(-1, 0);
                current_direction = sf::Vector2f(-1, 0);
                key_buffer = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && key_buffer == false && current_direction.x != -1) {
                the_snake.updateMovement(1, 0);
                current_direction = sf::Vector2f(1, 0);
                key_buffer = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && key_buffer == false && current_direction.y != -1) {
                the_snake.updateMovement(0, 1);
                current_direction = sf::Vector2f(0, 1);
                key_buffer = true;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
                the_snake.update_length();
            }

            the_snake.updateMovement(current_direction.x, current_direction.y);
            if (the_snake.get_location(0).x > window.getSize().x - 10 || the_snake.get_location(0).y > window.getSize().y - 10 || the_snake.get_location(0).y < 0 || the_snake.get_location(0).x < 0) {
                endGame(window);
            }
            for (int i = 1; i < the_snake.size() - 1; i++)
            {
                std::optional<sf::Rect<float>> intersect = the_snake.get_body_part(0).getGlobalBounds().findIntersection(the_snake.get_body_part(i).getGlobalBounds());
                if (intersect.has_value())
                {
                    int area = (intersect.value().size.x * intersect.value().size.y);
                    if (area > 70)
                    {
                        endGame(window);
                        isFrozen = true;
                    }
                }
            }

            window.draw(the_snake);

            // End the current frame and display its contents on screen
            window.display();
            window.clear();
        }
        // Event processing
        while (const std::optional event = window.pollEvent())
        {
            // Request for closing the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }
    }

}