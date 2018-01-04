#include "stdafx.h"
#include "text_block.h"


text_block::text_block(int xpos_,  int ypos_,
                       int width_, int height_) : clickable_entity(xpos_,ypos_,width_,height_){
	if (!font_type.loadFromFile("arial.ttf"))
		std::cout << "couldn't load font" << std::endl;
	current_text.setFont(font_type);
	current_text.setCharacterSize(30);
	current_text.setFillColor(sf::Color::Black);
	current_text.setPosition(sf::Vector2f(xpos, ypos));
	text_container.setFillColor(sf::Color::White);
	text_container.setSize(sf::Vector2f(width, height));
	text_container.setPosition(sf::Vector2f(xpos, ypos));
}


text_block::~text_block()
{
}

void text_block::set_text(const std::string& text) {
	current_text.setString(text);
}

bool text_block::pointer_clicks_block(const int& xpos_, const int& ypos_)const {
	return xpos_ >= xpos && xpos_ <= xpos + width && ypos_ >= ypos && ypos_ <= ypos + height;
}

std::string text_block::get_text()const {
	return current_text.getString();
}

void text_block::draw(sf::RenderWindow* window)const {
	window->draw(text_container);
	window->draw(current_text);
}