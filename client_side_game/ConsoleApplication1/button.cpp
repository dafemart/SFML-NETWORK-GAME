#include "stdafx.h"
#include "button.h"
#define space 90


button::button(button_parameters parameters) : clickable_entity(parameters.xpos_,parameters.ypos_,
	                                                            parameters.width_, parameters.height_)
{
	button_color = parameters.button_color;
	if (!font_type.loadFromFile("arial.ttf"))
		std::cout << "couldn't load font" << std::endl;
	button_text.setFont(font_type);
	button_text.setCharacterSize(30);
	button_text.setString(parameters.button_text);
	button_text.setFillColor(sf::Color::Black);
	button_text.setPosition(sf::Vector2f(xpos + space, ypos));
	button_shape.setFillColor(button_color);
	button_shape.setSize(sf::Vector2f(width, height));
	button_shape.setPosition(sf::Vector2f(xpos, ypos));
}


button::~button()
{
}



void button::draw(sf::RenderWindow* window){
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) == true) {
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
		if (this->pointer_clicks_block(mousePosition.x, mousePosition.y) == true) {
			button_shape.setFillColor(sf::Color::Yellow);
		}
	}
	else button_shape.setFillColor(button_color);
	window->draw(button_shape);
	window->draw(button_text);
}
