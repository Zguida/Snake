#ifndef BERRIES_H
#define BERRIES_H
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <map>
#include "snake.h"

class berries : public sf::Drawable{
public:
	berries(std::filesystem::path file, sf::Vector2u size);
	std::pair<sf::Sprite, int> get_sprite(int index);
	void new_berry(sf::Vector2u berry_location);
	int number_of_berries();
	bool check_new();
	void remove_new();
	float get_berry_size();
	void addBerry(berries& berry, sf::RectangleShape& window, int edge_difference, float delta, int piece_size, snake current_snake);
	void clearBerries();
	void removeBerry(int index);
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	sf::Texture sprite_texture;
	std::map<int, std::optional<sf::Sprite>> berry_vector;
	sf::Sprite berry = sf::Sprite(sprite_texture);
};

#endif // BERRIES_H
