#include "stdafx.h"
#include "player.h"

player::player(int xpos_, int ypos_) : xpos(xpos_), ypos(ypos_) {

}

void player::moveLeft() {
	xpos = ((xpos - 1) + 8) % 8;
}

void player::moveRight() {
	xpos = (xpos+1) % 8;
}

void player::moveUp() {
	ypos = ((ypos - 1) + 8) % 8;
}

void player::moveDown() {
	ypos = (ypos + 1) % 8;
}

int player::get_xpos()const {
	return xpos;
}

int player::get_ypos()const {
	return ypos;
}

void player::setCoordinates(int xpos_, int ypos_) {
	xpos = xpos_;
	ypos = ypos_;
}

bool player::operator==(const player& p2) {
	return xpos == p2.xpos && ypos == p2.ypos;
}
