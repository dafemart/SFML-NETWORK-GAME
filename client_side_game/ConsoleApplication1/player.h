#pragma once
#include <SFML/Graphics.hpp>


class player{
 private:
	 int xpos;
	 int ypos;
	 sf::CircleShape checker;
 public:
	player() = default;
	player(int xpos_, int ypos_, std::string what_entity, std::string player_number);
	~player() = default;

	void setShapePosition();
	void setCoordinates(int xpos_, int ypos_);

	int get_xpos()const;
	int get_ypos()const;

	bool operator==(const player& p2)const;

	sf::CircleShape& getPlayerShape();
};

