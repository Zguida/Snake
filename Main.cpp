#include <SFML/Graphics.hpp>
#include "Snake.h"
#include "Berries.h"
#include <iostream>

//bool for pausing game
bool paused = false;

sf::FloatRect static endGame(sf::RenderWindow& window) {
    sf::Font font("resources/game_over.ttf");
    float fontscale = (float)window.getSize().x / window.getSize().x*1.2f;
    sf::Text game_over = sf::Text(font, "GAME OVER", (int)(100/ fontscale));
    game_over.setPosition(sf::Vector2f(((window.getSize().x / 2) - game_over.getLocalBounds().size.x / 2), ((window.getSize().y / 3) - game_over.getLocalBounds().size.y / 2)));
    sf::Text play_again = sf::Text(font, "Play Again? (Spacebar)", (int)(50/ fontscale));
    float gox = game_over.getGlobalBounds().position.x;
    float goy = game_over.getGlobalBounds().position.y;
    play_again.setPosition(sf::Vector2f(gox + ((game_over.getGlobalBounds().size.x - play_again.getLocalBounds().size.x)/2), goy + 20));
    window.draw(game_over);
    window.draw(play_again);
    paused = true;
    window.display();
    window.clear();
    return play_again.getGlobalBounds();
}

int main()
{
    //variables
    int snake_size = 40;
    float snake_speed = snake_size * 8;
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u((int)(snake_size * 20u), (int)(snake_size * 20u)),32), "Snake");
    sf::Vector2u window_size = window.getSize();

    //inits
    sf::Vector2u size_vector(snake_size, snake_size);
    sf::Vector2f size_vector_float(size_vector);
    snake the_snake(size_vector);
    float relative_position = snake_size * .8f;
    int to_remove = -1;
    sf::Clock clock;
    berries berry("resources/Bush_blue_flowers3.png", size_vector); //generates the berry texture
    

    int key_buffer = 0; //used to isolate button presses

    sf::Vector2f current_direction; //direction the snake is facing

    sf::FloatRect play_button; //used for game over screen to reset game
    
    sf::RectangleShape bounding_box(sf::Vector2f(snake_size * 19, snake_size * 19));
    bounding_box.setOutlineColor(sf::Color::Red);
    bounding_box.setOutlineThickness(2);
    bounding_box.setPosition(sf::Vector2f(size_vector.x/2, size_vector.y/2));
    bounding_box.setFillColor(sf::Color::Transparent);
    float edge_width = (window.getSize().x - bounding_box.getSize().x) / 2;

    bool moved = false;
    
    while (window.isOpen())
    {

        while (!paused)
        {
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

            //accepts keybaord input and updates movement direction
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && key_buffer == false && current_direction.y != 1 && the_snake.get_body_part(0).getPosition() != size_vector_float)
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
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && key_buffer == false && current_direction.x != 1 && the_snake.get_body_part(0).getPosition() != size_vector_float)
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
            if (headx > bounding_box.getSize().x + edge_width || heady > bounding_box.getSize().y + edge_width || heady < (snake_size / 2) + edge_width || headx < (snake_size / 2) + edge_width)
            {

                play_button = endGame(window);
          
                break;
            }
            else
            {
                window.clear();
            }
         
            //checks collisons with body parts
            if (moved) {
                sf::Vector2f head_position = the_snake.get_body_part(0).getPosition();
                int snake_length = the_snake.size();
                for (int i = 2; i < snake_length - 1; i++)
                {
                    sf::Vector2f body_position = the_snake.get_body_part(i).getPosition();
                    if (((head_position.x < body_position.x + relative_position && head_position.x > body_position.x - relative_position) && (head_position.y < body_position.y + relative_position && head_position.y > body_position.y - relative_position)))
                    {
                        play_button = endGame(window);
                    }
                }
            }

            if (moved) {
                int a = berry.number_of_berries();
                for (int i = 0; i < a; i++)
                {
                    std::pair current_berry = berry.get_sprite(i);
                    sf::Vector2f berry_position = current_berry.first.getPosition();
                    if (current_berry.first.getGlobalBounds().findIntersection(the_snake.get_body_part(0).getGlobalBounds()) && moved == true)
                    {
                        to_remove = current_berry.second;
                        the_snake.update_length();
                        moved = false;
                    }
                }
            }
            if (to_remove != -1)
            {
                berry.removeBerry(to_remove);
                to_remove = -1;
            }

            //add a berry every 15 seconds
            //TODO: remove hardcoded timer
            if (berry.number_of_berries() <= 10)
            {
                static float accumulator = 0;
                accumulator += delta;
                int threshold = 1.7;
                    if (accumulator > threshold)
                    {
                        berry.addBerry(berry, bounding_box, edge_width, delta, snake_size, the_snake);
                        accumulator = 0;
                    }
            }

            static float fps_counter = 0;
            fps_counter += delta;
            if (delta > 1)
            {
                fps_counter = 0;
                
            }

            // End the current frame and display its contents on screen
            window.draw(berry);
            window.draw(the_snake);//draws snake on screen
            window.draw(bounding_box);
            window.display();
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
                    the_snake = snake(size_vector);
                    current_direction = sf::Vector2f(0, 0);
                    moved = false;
                    berry.clearBerries();
                    clock.reset();
                }
            }
        }
    }

}