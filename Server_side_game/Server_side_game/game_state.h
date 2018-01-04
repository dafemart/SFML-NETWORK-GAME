#include <iostream>
#include "player.h"
#define WIDTH 8
#define HEIGHT 8
class game_state {
private:
	bool board_state[WIDTH][HEIGHT];
public:
	game_state();
	~game_state() = default;
	bool check_collision(player* pl);
	bool check_empty_board();
	void set_chip_pos(int xpos, int ypos);
	void unset_chip_pos(int xpos, int ypos);

};