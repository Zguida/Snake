#include <deque>
#include <SFML/Graphics.hpp>
#ifndef SNAKE_H
#define SNAKE_H

class snake : public sf::Drawable
{
public:
	snake(sf::Vector2f initial_location);
	void updateMovement(int x, int y); //update movement on the x y plane
	sf::Vector2f get_location(int body_part);
	void update_length();
	int size();
	sf::RectangleShape get_body_part(int body_part);
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	std::vector<sf::RectangleShape> snake_body;
};

#endif SNAKE_H