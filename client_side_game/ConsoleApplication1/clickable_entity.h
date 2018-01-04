#pragma once
class clickable_entity
{
protected:
	int xpos;
	int ypos;
	int width;
	int height;

public:
	clickable_entity() = default;
	clickable_entity(int xpos_, int ypos_, int width_, int height_);
	bool pointer_clicks_block(const int& xpos_, const int& ypos_)const;
	~clickable_entity();
};

