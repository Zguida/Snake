#include "Berries.h"

berries::berries(std::filesystem::path file, sf::Vector2f piece_size) {
	try
	{
		if (!sprite_texture.loadFromFile(file))
		{
			throw std::runtime_error("No File loaded");
		}
	}
	catch (const std::exception& e)
	{

	}
	int x = sprite_texture.getSize().x;
	int y = sprite_texture.getSize().y;
	float size = piece_size.x;
	float factorx = size / x;
	float factory = size / y;
	berry.setTexture(sprite_texture);
	berry.setTextureRect(sf::IntRect(sf::Vector2i(0,0), sf::Vector2i(x, y)));
	berry.setOrigin(sf::Vector2f(x/2, y/2));
	berry.setScale(sf::Vector2f(factorx, factory));
}
sf::Sprite berries::get_sprite(int index) {
	return berry_vector[index];
}

void berries::new_berry(sf::Vector2f berry_location) {
	sf::Sprite berry_copy = berry;
	berry_copy.setPosition(sf::Vector2f(berry_location.x, berry_location.y));
	berry_vector.push_back(berry_copy);
}
int berries::number_of_berries() {
	return berry_vector.size();
}
void berries::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& berry : berry_vector)
	{
		target.draw(berry, states);
	}
}

bool berries::check_last()
{
	sf::Sprite& new_berry = berry_vector[berry_vector.size() - 1];
	for (const auto& berry : berry_vector) 
	{
		if (new_berry.getGlobalBounds().findIntersection(berry.getGlobalBounds()) && !(&new_berry == &berry))
		{
			return true;
		}
	}
	return false;
}

void berries::remove_last()
{
	berry_vector.pop_back();
}

int berries::get_berry_size()
{
	return berry.getLocalBounds().size.x;
}