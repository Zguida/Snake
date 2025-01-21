#include "Snake.h"

snake::snake(sf::Vector2u size)
{	
	sf::RectangleShape square = sf::RectangleShape(sf::Vector2f(size));
	square.setFillColor(sf::Color::Red);
	square.setOrigin(sf::Vector2f(size.x / 2, size.y / 2));
	square.setPosition(sf::Vector2f(size.x, size.y));
	snake_body.push_back(square);
}

bool snake::updateMovement(sf::Vector2f dir, float speed, float delta)
{
	static float accumulator = 0.0f;
	float step = snake_body[0].getSize().x;
	float interval = step / speed;

	sf::Vector2f initial = snake_body[0].getPosition();
	sf::Vector2f hold;
	sf::Vector2f move = sf::Vector2f(step * dir.x, step * dir.y);

	accumulator += delta;

	if (!snake_body.empty())
	{
		if (accumulator >= interval && dir.length() != 0)
		{
			accumulator = 0;
			snake_body[0].move(move);

			for (int i = 1; i < snake_body.size(); i++)
			{
				hold = snake_body[i].getPosition();
				snake_body[i].setPosition(initial);
				initial = hold;
			}
			return true;
		}

	}
	return false;
}

void snake::update_length() {
	sf::RectangleShape new_rect = snake_body[snake_body.size()-1];
	new_rect.setFillColor(sf::Color::Green);
	new_rect.setFillColor(sf::Color(std::rand() % 256, std::rand() % 256, std::rand() % 256, 255));
	snake_body.push_back(new_rect);
}

sf::Vector2f snake::get_location(int body_part)
{
	return snake_body[body_part].getPosition();
}

int snake::size() {
	return snake_body.size();
}

sf::RectangleShape& snake::get_body_part(int body_part) {
	return snake_body[body_part];
}

void snake::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& segment : snake_body) {
		target.draw(segment, states);
	}
}

