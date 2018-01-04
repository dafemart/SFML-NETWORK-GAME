#include "stdafx.h"
#include "clickable_entity.h"


clickable_entity::clickable_entity(int xpos_, int ypos_, int width_, int height_)
{
	xpos = xpos_;
	ypos = ypos_;
	width = width_;
	height = height_;
}


clickable_entity::~clickable_entity()
{
}

bool clickable_entity::pointer_clicks_block(const int& xpos_, const int& ypos_)const {
	return xpos_ >= xpos && xpos_ <= xpos + width && ypos_ >= ypos && ypos_ <= ypos + height;
}
