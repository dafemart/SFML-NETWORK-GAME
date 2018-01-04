#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "clickable_entity.h"
class text_block : public clickable_entity
{
private:
	sf::Font font_type;
	sf::Text current_text;
	sf::RectangleShape text_container;
public:
	text_block() = default;
	text_block(int xpos_, int ypos_, int width, int height);
	~text_block();

	void set_text(const std::string& text_to_set);
	std::string get_text()const;
	bool pointer_clicks_block(const int& xpos_, const int& ypos)const;
	void draw(sf::RenderWindow* window)const;
};


