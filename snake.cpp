#include "snake.h"

snake::snake(sf::Vector2f size)
{	
	sf::RectangleShape square = sf::RectangleShape(size);
	square.setFillColor(sf::Color::Green);
	square.setPosition(sf::Vector2f(50, 50));
	snake_body.push_back(square);
}

void snake::updateMovement(int x, int y)
{
	for (int i = snake_body.size()-1; i > 0; i--) {
		snake_body[i].setPosition(snake_body[i - 1].getPosition());
	}
	if (!snake_body.empty())
	{
		snake_body[0].setPosition(sf::Vector2f(snake_body[0].getPosition().x + (x*5), snake_body[0].getPosition().y + (y*5)));
	}
}

void snake::update_length() {
	sf::RectangleShape new_rect = snake_body[snake_body.size()-1];
	snake_body.push_back(new_rect);
}

sf::Vector2f snake::get_location(int body_part)
{
	return snake_body[body_part].getPosition();
}

int snake::size() {
	return snake_body.size();
}

sf::RectangleShape snake::get_body_part(int body_part) {
	return snake_body[body_part];
}

void snake::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& segment : snake_body) {
		target.draw(segment, states);
	}
}

