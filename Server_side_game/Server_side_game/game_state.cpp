#include "stdafx.h"
#include "game_state.h"

game_state::game_state() {
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; ++j) {
			board_state[i][j] = false;
		}
	}
}
bool game_state::check_collision(player* pl) {
	return board_state[pl->get_xpos()][pl->get_ypos()];
}
void game_state::set_chip_pos(int xpos, int ypos) {
	board_state[xpos][ypos] = true;
}
void game_state::unset_chip_pos(int xpos, int ypos) {
	board_state[xpos][ypos] = false;
}
bool game_state::check_empty_board() {
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; ++j) {
			if (board_state[i][j] == true)
				return false;
		}
	}
	return true;
}