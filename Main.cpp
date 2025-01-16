#include <SFML/Graphics.hpp>
#include "Snake.h"
#include "Berries.h"
#include <iostream>

//bool for pausing game
bool paused = false;

sf::FloatRect static endGame(sf::RenderWindow& window) {
    sf::Font font("resources/game_over.ttf");
    float fontscale = (float)window.getSize().x / window.getSize().x*1.4f;
    sf::Text game_over = sf::Text(font, "GAME OVER", (int)(100/ fontscale));
    game_over.setPosition(sf::Vector2f(((window.getSize().x / 2) - game_over.getLocalBounds().size.x / 2), ((window.getSize().y / 3) - game_over.getLocalBounds().size.y / 2)));
    sf::Text play_again = sf::Text(font, "Play Again? (Spacebar)", (int)(50/ fontscale));
    float gox = game_over.getGlobalBounds().position.x;
    float goy = game_over.getGlobalBounds().position.y;
    play_again.setPosition(sf::Vector2f(gox + ((game_over.getGlobalBounds().size.x - play_again.getLocalBounds().size.x)/2), goy + 20));
    window.draw(game_over);
    window.draw(play_again);
    paused = true;
    return play_again.getGlobalBounds();
}

void static addBerry(berries& berry, sf::RectangleShape& window, int edge_difference)
{
    int window_size = window.getSize().x;
    int randx = std::rand();
    int randy = std::rand();
    int diff = edge_difference / 2;
    int berry_size = berry.get_berry_size();
    int bounds = diff + berry_size;
    int xberry = bounds + (randx % (window_size - (bounds - 1)));
    int yberry = bounds + (randy % (window_size - (bounds - 1)));
    sf::Vector2f berry_location(xberry, yberry);
    berry.new_berry(berry_location);
    if (berry.check_last())
    {
        berry.remove_last();
        addBerry(berry, window, edge_difference);
    }
}



int main()
{
    //variables
    float snake_size = 30;
    float snake_speed = snake_size * 10;
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u((int)(snake_size * 20u), (int)(snake_size * 20u)),32), "Snake");
    sf::Vector2u window_size = window.getSize();

    //inits
    sf::Vector2f size_vector = sf::Vector2f(snake_size, snake_size);
    snake the_snake = snake(size_vector);
    sf::Clock clock;
    berries berry = berries("resources/Bush_blue_flowers3.png", size_vector); //generates the berry texture
    

    int key_buffer = 0; //used to isolate button presses

    sf::Vector2f current_direction; //direction the snake is facing

    sf::FloatRect play_button; //used for game over screen to reset game
    
    sf::RectangleShape bounding_box(sf::Vector2f(snake_size * 19, snake_size * 19));
    bounding_box.setOutlineColor(sf::Color::Red);
    bounding_box.setOutlineThickness(2);
    bounding_box.setPosition(sf::Vector2f(size_vector.x/2, size_vector.y/2));
    bounding_box.setFillColor(sf::Color::Transparent);
    
    while (berry.number_of_berries() < 10)
    {
        addBerry(berry, bounding_box, window.getSize().x - bounding_box.getSize().x);
    }

    bool moved = false;
    
    while (window.isOpen())
    {

        while (!paused)
        {
            window.draw(bounding_box);
            //delta time for movement
            sf::Time elapsed = clock.restart();
            float delta = elapsed.asSeconds();

            // Event processing
            while (const std::optional event = window.pollEvent())
            {
                // Request for closing the window
                if (event->is<sf::Event::Closed>())
                    window.close();
                if (event->is<sf::Event::KeyReleased>())
                    key_buffer = false;
            }

            //TODO: replace with bery collection logic 
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            {
                the_snake.update_length();
            }

            //accepts keybaord input and updates movement direction
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && key_buffer == false && current_direction.y != 1 && the_snake.get_body_part(0).getPosition() != size_vector)
            {
                //the_snake.collapse();
                current_direction = (sf::Vector2f(0, -1));
                moved = the_snake.updateMovement(current_direction, snake_speed, delta);
                key_buffer = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && key_buffer == false && current_direction.y != -1)
            {
                //the_snake.collapse();
                current_direction = (sf::Vector2f(0, 1));
                moved = the_snake.updateMovement(current_direction, snake_speed, delta);
                key_buffer = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && key_buffer == false && current_direction.x != 1 && the_snake.get_body_part(0).getPosition() != size_vector)
            {
                //the_snake.collapse();
                current_direction = (sf::Vector2f(-1, 0));
                moved = the_snake.updateMovement(current_direction, snake_speed, delta);
                key_buffer = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && key_buffer == false && current_direction.x != -1)
            {
                //the_snake.collapse();
                current_direction = (sf::Vector2f(1, 0));
                moved = the_snake.updateMovement(current_direction, snake_speed, delta);
                key_buffer = true;
            }
            else if (current_direction.length() != 0)
            {
                moved = the_snake.updateMovement(current_direction, snake_speed, delta); //continues movement in current direction
            }

            //checks if the snake has gone out of bounds
            float headx = the_snake.get_location(0).x;
            float heady = the_snake.get_location(0).y;
            if (headx > window.getSize().x - snake_size / 2 || heady > window.getSize().y - snake_size/2 || heady < snake_size/2 || headx < snake_size/2)
            {
                play_button = endGame(window);
            }
         
            
            //checks collisons with body parts
            if (moved) {
                sf::Vector2f head_position = the_snake.get_body_part(0).getPosition();
                for (int i = 2; i < the_snake.size() - 1; i++)
                {
                    sf::Vector2f body_position = the_snake.get_body_part(i).getPosition();
                    float relative_position = snake_size*.8f;
                    if (((head_position.x < body_position.x + relative_position && head_position.x > body_position.x - relative_position) && (head_position.y < body_position.y + relative_position && head_position.y > body_position.y - relative_position)))
                    {
                        play_button = endGame(window);
                    }
                }
            }
            // End the current frame and display its contents on screen
            window.display();
            window.clear();
            window.draw(the_snake); //draws snake on screen
            window.draw(berry);
        }
        // Event processing for after game over
        while (const std::optional event = window.pollEvent())
        {
            // Request for closing the window
            if (event->is<sf::Event::Closed>())
                window.close();
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) 
            {
                sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                if (play_button.contains(sf::Vector2f((float)mouse_pos.x, (float)mouse_pos.y)) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
                {
                    paused = false;
                    window.clear();
                    the_snake = snake(sf::Vector2f(snake_size, snake_size));
                    current_direction = sf::Vector2f(0, 0);
                    moved = false;
                }
            }
        }
    }

}