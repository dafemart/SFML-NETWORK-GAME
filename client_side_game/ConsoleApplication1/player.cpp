#include "stdafx.h"
#include "player.h"

std::string MONSTER_ENTITY = "monster";
std::string PLAYER_ENTITY = "player";

player::player(int xpos_, int ypos_, std::string what_entity, std::string player_number) : xpos(xpos_), ypos(ypos_){ 
	if (what_entity == MONSTER_ENTITY) {
		checker.setRadius(25);
		checker.setFillColor(sf::Color::Green);
	}
	else if (what_entity == PLAYER_ENTITY) {
		checker.setRadius(50);
		if(player_number == "1")
		   checker.setFillColor(sf::Color::Red);
		else
			checker.setFillColor(sf::Color::Blue);
	}
}

int player::get_xpos()const{
	return xpos;
}

int player::get_ypos()const{
	return ypos;
}

sf::CircleShape& player::getPlayerShape() {
	return checker;
}

void player::setShapePosition() {
	checker.setPosition(xpos, ypos);
}

void player::setCoordinates(int xpos_, int ypos_) {
	xpos = xpos_;
	ypos = ypos_;
}

bool player::operator==(const player& p2)const {
	return xpos == p2.xpos && ypos == p2.ypos;
}
