#pragma once
#include <SFML/Graphics.hpp>
class entity{
protected:
	int xpos;
	int ypos;
	sf::CircleShape checker;
public:
	entity() = delete;
	~entity();
};

