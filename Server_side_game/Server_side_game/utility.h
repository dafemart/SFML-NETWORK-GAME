#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "game_state.h"

struct game_packet {
	std::string type;
	std::string player_move;
	double chip_position_x; // change it to string
	double chip_position_y;
	double pl1_position_x;
	double pl1_position_y;
	double pl2_position_x;
	double pl2_position_y;
	int score_pl1;
	int score_pl2;
};

struct listen_param {
	sf::TcpSocket* listener;
	std::string* data;
	bool* send_player_move;
	bool* send_chip_deletion;
	bool* send_chip_creation;
	double* chip_pos_x;
	double* chip_pos_y;
	player* player1;
	player* player2;
	game_state* gameState;
	std::string* chip_coordinates;
	int* score_pl1;
	int* score_pl2;
};

sf::Packet& operator <<(sf::Packet& packet, const game_packet& pack){
	return packet << pack.type << pack.player_move << pack.chip_position_x << pack.chip_position_y
		<< pack.pl1_position_x << pack.pl1_position_y << pack.pl2_position_x << pack.pl2_position_y
		<< pack.score_pl1 << pack.score_pl2;
}          

sf::Packet& operator >>(sf::Packet& packet, game_packet& pack){
	return packet >> pack.type >> pack.player_move >> pack.chip_position_x >> pack.chip_position_y
		>> pack.pl1_position_x >> pack.pl1_position_y >> pack.pl2_position_x >> pack.pl2_position_y
		>> pack.score_pl1 >> pack.score_pl2;
}