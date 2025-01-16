#ifndef BERRIES_H
#define BERRIES_H
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

class berries : public sf::Drawable{
public:
	berries(std::filesystem::path file, sf::Vector2f size);
	sf::Sprite get_sprite(int index);
	void new_berry(sf::Vector2f berry_location);
	int number_of_berries();
	bool check_last();
	void remove_last();
	int get_berry_size();
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	sf::Texture sprite_texture;
	std::vector<sf::Sprite> berry_vector;
	sf::Sprite berry = sf::Sprite(sprite_texture);
};

#endif // BERRIES_H
