// Serversidegame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>  
#include <sstream>
#include "utility.h"

const std::string PLAYER_MOVE_REQUEST = "PLAYER_MOVE";
const std::string CHIP_CREATION_REQUEST = "CHIP_CREATION";
const std::string CHIP_DELETION_REQUEST = "CHIP_DELETION";

/*This function handles chip creation on board. Since the dimensions of the game board is 8 X 7, this function
generates 15 random numbers between 0 and 56 and then these numbers are mapped to the right coordinates on the
game board. Once these random numbers are generated, the function triggers a boolean variable  which 
wakes up an if statement in another thread. This if statement sends a packet to the client with the generated
random numbers which are tranlated to graphics by the client side.*/

void handle_chip_creation(std::string* data, bool* send_chip_creation, game_state* gameState) {
	int NUMBER_OF_RANDS = 15; //Number of chips on board
	int rand_number; 
	std::string position_chips;
	srand(time(NULL));
	int mapped_chip_xpos;
	int mapped_chip_ypos;
	std::cout << "generating board numbers" << std::endl;
	for (int i = 0; i < NUMBER_OF_RANDS; i++) {
		std::stringstream string_rand;
		rand_number = rand() % 55 + 0;
		std::cout << rand_number << std::endl;
		string_rand << rand_number;
		position_chips += string_rand.str();
		position_chips += "_";
		mapped_chip_xpos = rand_number % 8;
		mapped_chip_ypos = rand_number / 8;
		gameState->set_chip_pos(mapped_chip_xpos, mapped_chip_ypos);
	}
	std::cout << std::endl;
	*(data) = position_chips;
	std::cout << "sending coordinates" << *(data) << std::endl;
	*(send_chip_creation) = true;
}

/*This function handles the deletion of a chip if a move request results in collision*/

void handle_chip_deletion(int chip_xpos, int chip_ypos, listen_param param) {
	*(param.chip_pos_x) = chip_xpos;
	*(param.chip_pos_y) = chip_ypos;
	*(param.send_chip_deletion) = true;
}

/*Handles move request. Check for possible collisions from both players. Check whether the board is empty or not
after a player-move request*/
void handle_plmove_request(game_packet packet, listen_param param) {
	if (packet.player_move == "u1")
		param.player1->moveUp();
	else if (packet.player_move == "d1")
		param.player1->moveDown();
	else if (packet.player_move == "l1")
		param.player1->moveLeft();
	else if (packet.player_move == "r1")
		param.player1->moveRight();
	else if (packet.player_move == "u2")
		param.player2->moveUp();
	else if (packet.player_move == "d2")
		param.player2->moveDown();
	else if (packet.player_move == "l2")
		param.player2->moveLeft();
	else if (packet.player_move == "r2")
		param.player2->moveRight();	

	double delete_xpos;
	double delete_ypos;

	if (param.gameState->check_collision(param.player1) == true) {
		delete_xpos = param.player1->get_xpos();  
		delete_ypos = param.player1->get_ypos();
		param.gameState->unset_chip_pos(delete_xpos, delete_ypos);
		handle_chip_deletion(delete_xpos,delete_ypos,param);
		*(param.score_pl1) = *(param.score_pl1) + 1;
	}
	if (param.gameState->check_collision(param.player2) == true) {
		delete_xpos = param.player2->get_xpos();
		delete_ypos = param.player2->get_ypos();
		param.gameState->unset_chip_pos(delete_xpos, delete_ypos);
		handle_chip_deletion(delete_xpos, delete_ypos, param);
		*(param.score_pl2) = *(param.score_pl2) + 1;
	}
	if (param.gameState->check_empty_board() == true)
		handle_chip_creation(param.chip_coordinates, param.send_chip_creation, param.gameState);

	*(param.data) = packet.player_move;
	*(param.send_player_move) = true;
}

/*Listen_to_client forwards all the requests to the right function*/

void listen_to_client(listen_param param) {
	while (true) {
		sf::Packet packet;
		game_packet gamePacket;
		if (param.listener->receive(packet) != sf::Socket::Done)
			std::cout << "awaiting for input" << std::endl;
		packet >> gamePacket;
		if (gamePacket.type == PLAYER_MOVE_REQUEST)
			handle_plmove_request(gamePacket, param);
	}
}



int main()
{
	//--------------------------------Handles network connection
	sf::TcpListener listener_player1;
	sf::TcpListener listener_player2;
	
	if (listener_player1.listen(53005) != sf::Socket::Done) 
		std::cerr << "error binding player1 to the port" << std::endl;
	sf::TcpSocket client;
	if (listener_player1.accept(client) != sf::Socket::Done)
		std::cerr << "no connection" << std::endl;
	player player1(0,7);
	std::string player_number = "1";
	sf::Packet packet;
	packet << player1.get_xpos() << player1.get_ypos() << player_number;
	if (client.send(packet) != sf::Socket::Done)
		std::cout << "couldn't send packet" << std::endl;

	if (listener_player1.listen(53005) != sf::Socket::Done)
		std::cerr << "error binding player 2 to the sockect" << std::endl;
	sf::TcpSocket client2;
	if (listener_player1.accept(client2) != sf::Socket::Done)
		std::cerr << "no connection" << std::endl;
	player player2(7,7);
	player_number = "2";
	sf::Packet packet2;
	packet2 << player2.get_xpos() << player2.get_ypos() << player_number;
	if (client2.send(packet2) != sf::Socket::Done)
		std::cout << "couldn't send packet" << std::endl;
	//-----------------------------------------------------

	//--------------------------------------core variables
	game_state gameState; //Object that handles the current state of the game
	bool send_plmoveone = false;  //Triggers a request for player1 moves
	bool send_plmovetwo = false; //Triggers a request for player2 moves
	bool send_chip_deletion = false; //Triggers a request for chip_deletion
	bool send_chip_creation = false; //triggers a request for chip creation
	double chip_position_x;  //Variable used in the request context
	double chip_position_y; // variable used in the request context
	std::string chips_coordinates; // Variable used in the chip-creation request
	int score_pl1 = 0; // Current Score of  player1 after a request
	int score_pl2 = 0; // Current score of player2 after a request
	//-----------------------------------------

	std::cout << "sending chips coordinates" << std::endl;
	handle_chip_creation(&chips_coordinates, &send_chip_creation, &gameState);  

	std::string ins_player1;
	listen_param param_p1{ &client , &ins_player1, &send_plmoveone,
		                  &send_chip_deletion,&send_chip_creation, 
		                  &chip_position_x, &chip_position_y, 
		                  &player1, &player2, &gameState , &chips_coordinates,
	                      &score_pl1, &score_pl2};
	sf::Thread client_listener_thread_p1(&listen_to_client, param_p1);
	client_listener_thread_p1.launch();

	std::string ins_player2;
	listen_param param_p2{ &client2, &ins_player2, &send_plmovetwo,
	                       &send_chip_deletion,&send_chip_creation, 
		                   &chip_position_x, &chip_position_y, 
		                   &player1, &player2, &gameState, &chips_coordinates,
	                       &score_pl1, &score_pl2};
	sf::Thread client_listener_thread_p2(&listen_to_client, param_p2);
	client_listener_thread_p2.launch();

	
	while (true) {
		if (send_chip_creation == true) {
			sf::Packet packet;
			game_packet gamePacket{ CHIP_CREATION_REQUEST, chips_coordinates, 0 ,0};
			packet << gamePacket;
			std::cout << "sending coordinates" << chips_coordinates << std::endl;
			if (client.send(packet) != sf::Socket::Done)
				std::cout << "couldn't send packet" << std::endl;
			if (client2.send(packet) != sf::Socket::Done)
				std::cout << "couldn't send packet" << std::endl;
			send_chip_creation = false;
		}

		if (send_plmoveone == true) {
			sf::Packet packet;
			game_packet gamePacket{ PLAYER_MOVE_REQUEST, ins_player1, 0 , 0,
				                    player1.get_xpos(), player1.get_ypos(),
				                    player2.get_xpos(), player2.get_ypos(),
			                        score_pl1, score_pl2};
			packet << gamePacket;
			if (client.send(packet) != sf::Socket::Done)
				std::cout << "couldn't send packet" << std::endl;
			if (client2.send(packet) != sf::Socket::Done)
				std::cout << "couldn't send packet" << std::endl;
			send_plmoveone = false;
		}
		if (send_plmovetwo == true) {
			sf::Packet packet;
			game_packet gamePacket{ PLAYER_MOVE_REQUEST, ins_player1, 0 , 0,
				                    player1.get_xpos(), player1.get_ypos(),
				                    player2.get_xpos(), player2.get_ypos(),
			                        score_pl1, score_pl2};
			packet << gamePacket;
			if (client.send(packet) != sf::Socket::Done)
				std::cout << "couldn't send packet" << std::endl;
			if (client2.send(packet) != sf::Socket::Done)
				std::cout << "couldn't send packet" << std::endl;
			send_plmovetwo = false;
		}
		if (send_chip_deletion == true) {
			sf::Packet packet;
			game_packet gamePacket{CHIP_DELETION_REQUEST, "null", chip_position_x, chip_position_y};
			packet << gamePacket;
			if (client.send(packet) != sf::Socket::Done)
				std::cout << "couldn't send packet" << std::endl;
			if (client2.send(packet) != sf::Socket::Done)
				std::cout << "couldn't send packet" << std::endl;
			send_chip_deletion = false;
			chip_position_x = 0;
			chip_position_y = 0;
		}
		
	}

    return 0;
}

