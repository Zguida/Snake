#include <SFML/Graphics.hpp>
#include "snake.h"
#include <iostream>

//bool for pausing game
bool isFrozen = false;

sf::FloatRect endGame(sf::RenderWindow& window) {
    sf::Font font("resources/game_over.ttf");
    sf::Text game_over = sf::Text(font, "GAME OVER", 100);
    game_over.setPosition(sf::Vector2f(((window.getSize().x / 2) - game_over.getLocalBounds().size.x / 2), ((window.getSize().y / 3) - game_over.getLocalBounds().size.y / 2)));
    sf::Text play_again = sf::Text(font, "Play Again?", 50);
    int gox = game_over.getGlobalBounds().position.x;
    int goy = game_over.getGlobalBounds().position.y;
    play_again.setPosition(sf::Vector2f(gox + game_over.getGlobalBounds().size.x/4, goy + game_over.getGlobalBounds().size.y));
    window.draw(game_over);
    window.draw(play_again);
    isFrozen = true;
    return play_again.getGlobalBounds();
}

int main()
{
    // Declare and create a new window
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML window");

    // Limit the framerate to 60 frames per second (this step is optional)
    window.setFramerateLimit(60);

    // The main loop - ends as soon as the window is closed
    snake the_snake = snake(sf::Vector2f(10, 10));

    //used to isolate button presses
    bool key_buffer = NULL;

    //direction the snake is facing
    sf::Vector2f current_direction;
    //used for game over screen to reset game
    sf::FloatRect play_button;
    //used to offset collision detection when a new peice is added at the back of the snake
    bool length_updated = false;

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
            //accepts keybaord input and updates movement direction
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
            //remove in final build
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && current_direction.lengthSquared() != 0) {
                the_snake.update_length();
                length_updated = true;
            }
            //continues movement in current direction
            the_snake.updateMovement(current_direction.x, current_direction.y);

            //checks if the snake has gone out of bounds
            int headx = the_snake.get_location(0).x;
            int heady = the_snake.get_location(0).y;
            if (headx > window.getSize().x - 10 || heady > window.getSize().y - 10 || heady < 0 || headx < 0) {
                play_button = endGame(window);
            }
            //draws snake on screen
            window.draw(the_snake);
            //checks collisons with body parts
            for (int i = 1; i < the_snake.size() - 1; i++)
            {
                std::optional<sf::Rect<float>> intersect = the_snake.get_body_part(0).getGlobalBounds().findIntersection(the_snake.get_body_part(i).getGlobalBounds());
                if (intersect.has_value())
                {
                    play_button = endGame(window);
                }
            }

            // End the current frame and display its contents on screen
            window.display();
            window.clear();
            length_updated = false;
        }
        // Event processing for after game over
        while (const std::optional event = window.pollEvent())
        {
            // Request for closing the window
            if (event->is<sf::Event::Closed>())
                window.close();
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                if (play_button.contains(sf::Vector2f(mouse_pos.x, mouse_pos.y))) {
                    isFrozen = false;
                    window.clear();
                    the_snake = snake(sf::Vector2f(10, 10));
                    current_direction = sf::Vector2f(0, 0);
                }
            }
        }
    }

}