#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "clickable_entity.h"
struct button_parameters {
	int xpos_;
	int ypos_;
	int width_;
	int height_;
	std::string button_text;
	sf::Color button_color;
};
class button : public clickable_entity
{
private:
	sf::RectangleShape button_shape;
	sf::Text button_text;
	sf::Color button_color;
	sf::Font font_type;
public:
	button() = default;
	button(button_parameters parameters);
	~button();
	void draw(sf::RenderWindow* window);
};

