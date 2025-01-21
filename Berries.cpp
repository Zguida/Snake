#include "Berries.h"

berries::berries(std::filesystem::path file, sf::Vector2u piece_size) {
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
std::pair<sf::Sprite, int> berries::get_sprite(int index) 
{
	auto index_iter = berry_vector.find(index);
	int size = berry_vector.size();
	if (index_iter != berry_vector.end() && berry_vector[index].has_value() && index >= 0 && index < size)
	{
		return std::pair(berry_vector[index].value(), index);
	}
}

void berries::new_berry(sf::Vector2u berry_location) {
	sf::Sprite berry_copy = berry;
	bool added = false;
	berry_copy.setPosition(sf::Vector2f(berry_location));
	for (int i = 0; i < berry_vector.size(); i++)
	{
		if (berry_vector.find(i) == berry_vector.end() && added == false)
		{
			berry_vector.emplace(i, berry_copy);
			added = true;
		}
	}
	if (added == false)
	{
		berry_vector.emplace(berry_vector.size(), berry_copy);
	}
}
int berries::number_of_berries() {
	return berry_vector.size();
}
void berries::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (berry_vector.size() > 0)
	{
		for (const auto& berry : berry_vector)
		{
			if (berry.second.has_value())
			{
				target.draw(berry.second.value(), states);
			}
		}
	}
}

bool berries::check_new()
{
	int size = berry_vector.size();
	if (size > 0 && berry_vector[size - 1].has_value())
	{
		sf::Sprite& new_berry = berry_vector[size - 1].value();
		for (const auto& berry : berry_vector)
		{
			if (berry.second.has_value() && new_berry.getGlobalBounds().findIntersection(berry.second.value().getGlobalBounds()) && !(&new_berry == &berry.second.value()))
			{
				return true;
			}
		}
	}
	return false;
}

void berries::remove_new()
{
	int size = berry_vector.size();
	if (size > 0 && berry_vector[size - 1].has_value())
	{
		auto last = berry_vector.find(size - 1);
		berry_vector.erase(last);
	}
}

float berries::get_berry_size()
{
	return berry.getLocalBounds().size.x;
}

void berries::addBerry(berries& berry, sf::RectangleShape& window, int edge_difference, float delta, int piece_size, snake current_snake)
{
	int window_size = window.getSize().x;
	int randx = current_snake.get_body_part(0).getPosition().x + std::rand();
	int randy = current_snake.get_body_part(0).getPosition().y + std::rand();
	int berry_size = berry.get_berry_size();
	int bounds = edge_difference + berry_size;
	int xberry = bounds + (randx % (window_size - (bounds - 1)));
	int yberry = bounds + (randy % (window_size - (bounds - 1)));
	int true_x = xberry - (xberry % piece_size);
	int true_y = yberry - (yberry % piece_size);
	sf::Vector2u berry_location(true_x, true_y);
	int size2 = berry.number_of_berries();
	berry.new_berry(berry_location);

	 if (berry.check_new())
	{
		berry.remove_new();
		addBerry(berry, window, edge_difference, 0, piece_size, current_snake);
	}
}

void berries::clearBerries()
{
	berry_vector.clear();
}
void berries::removeBerry(int index)
{	
	int size = berry_vector.size();
	if (size > 0 && index < size)
	{
		auto p = berry_vector.find(index);
		berry_vector.erase(berry_vector.find(index));
	}
}