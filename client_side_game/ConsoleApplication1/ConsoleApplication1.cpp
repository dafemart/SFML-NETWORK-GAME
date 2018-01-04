// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <list>
#include <sstream>
#include <assert.h>
#include "player.h"
#include "text_block.h"
#include "button.h"


std::string PLAYER_MOVE_REQUEST = "PLAYER_MOVE";
std::string CHIP_CREATION_REQUEST = "CHIP_CREATION";
std::string CHIP_DELETION_REQUEST = "CHIP_DELETION";
std::string MONSTER_ENTITY_MY = "monster";
std::string PLAYER_ENTITY_MY= "player";

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

struct listen_parameters {
	sf::TcpSocket* socket;
	player* player1;
	player* player2;
	sf::RenderWindow* what_window;
	std::list<player>* chipList;
	bool* lock_movement;
	sf::Text* pl1_score;
	sf::Text* pl2_score;
};

struct interface_parameters {
	sf::TcpSocket* socket;
	sf::RenderWindow* window; 
	bool* connection;
};
sf::Packet& operator <<(sf::Packet& packet, const game_packet& pack)
{
	return packet << pack.type << pack.player_move << pack.chip_position_x << pack.chip_position_y
		<< pack.pl1_position_x << pack.pl1_position_y << pack.pl2_position_x << pack.pl2_position_y
		<< pack.score_pl1 << pack.score_pl2;
}

sf::Packet& operator >>(sf::Packet& packet, game_packet& pack)
{
	return packet >> pack.type >> pack.player_move >> pack.chip_position_x >> pack.chip_position_y
		>> pack.pl1_position_x >> pack.pl1_position_y >> pack.pl2_position_x >> pack.pl2_position_y
		>> pack.score_pl1 >> pack.score_pl2;
}

void handle_plmove_request(game_packet packet, listen_parameters param) {
	std::stringstream int_to_string_pl1;
	std::stringstream int_to_string_pl2;
	sf::String score_pl1_str;
	sf::String score_pl2_str;
	double set_xpos = packet.pl1_position_x * 100 + 50;
	double set_ypos = packet.pl1_position_y * 100 + 50;
	param.player1->setCoordinates(set_xpos, set_ypos);
	set_xpos = packet.pl2_position_x * 100 + 50;
    set_ypos = packet.pl2_position_y * 100 + 50;
	param.player2->setCoordinates(set_xpos, set_ypos);
	int_to_string_pl1 << packet.score_pl1;
	score_pl1_str = int_to_string_pl1.str();
	param.pl1_score->setString(score_pl1_str);
	int_to_string_pl2 << packet.score_pl2;
	score_pl2_str = int_to_string_pl2.str();
	param.pl2_score->setString(score_pl2_str);
}

void handle_chip_deletion(game_packet packet, listen_parameters param) {
	std::cout << "handling chip_deletion_rquest" << std::endl;
	packet.chip_position_x = packet.chip_position_x * 100 + 50;
	packet.chip_position_y = packet.chip_position_y * 100 + 50;
	player chip_to_delete(packet.chip_position_x, packet.chip_position_y, MONSTER_ENTITY_MY, "0");
	std::cout << "deleting" << packet.chip_position_x << ","
		      << packet.chip_position_y;
	param.chipList->remove(chip_to_delete);
}

void handle_chip_creation(game_packet packet, listen_parameters param) {
	*(param.lock_movement) = true;
	param.chipList->clear();
	std::string string_number;
	int new_integer;
	int xpos_newchip;
	int ypos_newchip;
	std::cout << "received integers" << std::endl;
	param.chipList->clear();
	for (auto& it : packet.player_move) {
		if (it != '_')
			string_number += it;
		else {
			std::stringstream string_to_int(string_number);
			string_to_int >> new_integer;
			std::cout << new_integer << ",";
			xpos_newchip = (new_integer % 8) * 100 + 50;
			ypos_newchip = (new_integer / 8) * 100 + 50;
			string_number.clear();
			player new_chip(xpos_newchip, ypos_newchip, MONSTER_ENTITY_MY , "0");
			param.chipList->push_back(new_chip);
		}

	}
	std::cout << std::endl;
	*(param.lock_movement) = false;
	
}
void listen_to_server(listen_parameters param){
	while (true) {
		sf::Packet packet;
		game_packet gamePacket;
		if (param.socket->receive(packet) != sf::Socket::Done)
			std::cout << "data couldn't be received" << std::endl;
		packet >> gamePacket;
		if (gamePacket.type == PLAYER_MOVE_REQUEST)
			handle_plmove_request(gamePacket, param);
		else if (gamePacket.type == CHIP_DELETION_REQUEST) {
			handle_chip_deletion(gamePacket, param);
		}
		else if (gamePacket.type == CHIP_CREATION_REQUEST) {
			handle_chip_creation(gamePacket, param);
		}
	}
}
void render_graphics(listen_parameters param) {
	sf::Clock clock;
	sf::Clock bot_clock;
	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
		std::cout << "can't load" << std::endl;
	sf::Text player1_text;
	sf::Text player2_text;
	player1_text.setFont(font);
	player1_text.setString("PLAYER_1 SCORE");
	player1_text.setFillColor(sf::Color::Red);
	player1_text.setPosition(150, 850);
	player2_text.setFont(font);
	player2_text.setString("PLAYER_2 SCORE");
	player2_text.setFillColor(sf::Color::Blue);
	player2_text.setPosition(500, 850);

	sf::RectangleShape white_square(sf::Vector2f(100, 100));
	white_square.setFillColor(sf::Color(0, 0, 0));
	sf::RectangleShape black_square(sf::Vector2f(100, 100));
	black_square.setFillColor(sf::Color(255, 255, 255));
		while (param.what_window->isOpen()) {
			float two = 2;
			sf::Time elapsed1 = clock.getElapsedTime();
			sf::Time bot_elapsed = bot_clock.getElapsedTime();
			sf::Time two_seconds = sf::seconds(2);
			sf::Time one_millisecond = sf::milliseconds(10);
			
			if (elapsed1 > one_millisecond) {
				param.what_window->clear();
				bool isBlack = true;
				for (int i = 0; i < 800; i += 100) {
					for (int j = 0; j < 800; j += 100) {
						if (isBlack == true) {
							black_square.setPosition(i + 50, j + 50);
							param.what_window->draw(black_square);
							if (j + 100 != 800)
								isBlack = false;
						}
						else {
							white_square.setPosition(i + 50, j + 50);
							param.what_window->draw(white_square);
							if (j + 100 != 800)
								isBlack = true;
						}
					}
				}
				//std::cout << "displaying child thread" << std::endl;
				param.player1->setShapePosition();
				param.what_window->draw(param.player1->getPlayerShape());
				param.player2->setShapePosition();
				param.what_window->draw(param.player2->getPlayerShape());
				param.what_window->draw(player1_text);
				param.what_window->draw(player2_text);
				param.what_window->draw(*(param.pl1_score)); // where the score is drawn
				param.what_window->draw(*(param.pl2_score)); // where the score is drawn

				for (auto& it : *(param.chipList)) {
					it.setShapePosition();
					param.what_window->draw(it.getPlayerShape());
				}

				param.what_window->display();
				clock.restart();
			}
		}
   
}

bool connect_to_server(sf::TcpSocket* socket) {
	std::cout << "initiating connection process" << std::endl;
	bool connection_status = false;
	int port_number = 53005;
	sf::Socket::Status status = socket->connect("192.168.137.1", port_number);
	if (status != sf::Socket::Done)
		std::cout << "awaiting connection" << std::endl;
	else {
		connection_status = true;
		std::cout << "succesfull connection" << std::endl;
	}
	std::cout << "returning" << std::endl;
	return connection_status;
}

void render_interface(interface_parameters parameters){
	parameters.window->setActive(true);
	sf::RectangleShape background(sf::Vector2f(900, 1000));
	background.setFillColor(sf::Color::Green);
	text_block username_block(300, 250, 300, 50);
	button_parameters connect_b_param{ 300,350,300,50,"connect",sf::Color::Red };
	button connect_button(connect_b_param);
	sf::Texture title;
	if (!title.loadFromFile("chips_title.png"))
		return;
	sf::Sprite contour;
	contour.setTexture(title);
	contour.setPosition(200, 100);
	sf::Vector2i position{ -500,-500 };
	bool mouse_pressed = false;
	while (parameters.window->isOpen()) {
		parameters.window->clear();
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			position = sf::Mouse::getPosition(*(parameters.window));
		}
		sf::Event my_event;
		while (parameters.window->pollEvent(my_event)) {
			if (my_event.type == sf::Event::Closed)
				parameters.window->close();
			else if (my_event.type == sf::Event::TextEntered) {
				if (username_block.pointer_clicks_block(position.x, position.y) == true) {
					std::string current_text_str = username_block.get_text();
					if (my_event.text.unicode >= 32 && my_event.text.unicode <= 126) {
						current_text_str += (char)my_event.text.unicode;
						username_block.set_text(current_text_str);
					}
					else if (my_event.text.unicode == 8 && current_text_str.size() > 0) {
						current_text_str.pop_back();
						username_block.set_text(current_text_str);
					}
				}
			}
		}

		if (connect_button.pointer_clicks_block(position.x, position.y) == true) {
			bool connection_status = connect_to_server(parameters.socket);
			if (connection_status == true) {
				parameters.window->setActive(false);
			    *(parameters.connection) = true;
				break;
			}
		}


		parameters.window->draw(background);
		parameters.window->draw(contour);
		username_block.draw(parameters.window);
		connect_button.draw(parameters.window);
		parameters.window->display();
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(900, 1000), "CHIPS");
	window.setKeyRepeatEnabled(false);
	sf::TcpSocket socket;
	bool connection = false;
	interface_parameters inter_parameters{ &socket, &window, &connection};
	render_interface(inter_parameters);
	window.setActive(false);

	sf::Packet packet;
	int xpos;
	int ypos;
	std::string player_number;
	if (socket.receive(packet) != sf::Socket::Done)
		std::cout << "can't receive packet" << std::endl;
	packet >> xpos >> ypos >> player_number;
	std::cout << "your player number is" << player_number << std::endl;


	
	player player1(50,750,PLAYER_ENTITY_MY, "1");
	player player2(750,750, PLAYER_ENTITY_MY, "2");
	sf::RenderWindow* ptr_window = &window;
	std::list<player> chipList;
	bool lock_movement = false;
	sf::Text pl1_score;
	sf::Text pl2_score;
	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
		std::cout << "can't load" << std::endl;
	pl1_score.setFont(font);
	pl1_score.setFillColor(sf::Color::Red);
	pl1_score.setPosition(250, 950);
	pl2_score.setFont(font);
	pl2_score.setFillColor(sf::Color::Blue);
	pl2_score.setPosition(600, 950);
	pl1_score.setString("0");
	pl2_score.setString("0");

	listen_parameters param{&socket, &player1, &player2, ptr_window,&chipList,&lock_movement,
	                        &pl1_score, &pl2_score};
	sf::Thread server_listener_thread{ &listen_to_server, param };
	server_listener_thread.launch();
	sf::Thread graphics(&render_graphics, param);
	graphics.launch();	

	
	while (window.isOpen()){
		sf::Event event;
		while (window.pollEvent(event))
		{
			std::string ins;
			sf::Packet packet;
			if (lock_movement == false) {
				switch (event.type)
				{
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
						game_packet gamePacket;
						gamePacket.type = PLAYER_MOVE_REQUEST;
						ins = "r" + player_number;
						gamePacket.player_move = ins;
						packet << gamePacket;
						if (socket.send(packet) != sf::Socket::Done)
							std::cout << "couldn't send packet" << std::endl;
						std::cout << "sending r instruction" << std::endl;
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
						game_packet gamePacket;
						gamePacket.type = PLAYER_MOVE_REQUEST;
						ins = "l" + player_number;
						gamePacket.player_move = ins;
						packet << gamePacket;
						if (socket.send(packet) != sf::Socket::Done)
							std::cout << "couldn't send packet" << std::endl;
						std::cout << "sending l instruction" << std::endl;
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
						game_packet gamePacket;
						gamePacket.type = PLAYER_MOVE_REQUEST;
						ins = "u" + player_number;
						gamePacket.player_move = ins;
						packet << gamePacket;
						if (socket.send(packet) != sf::Socket::Done)
							std::cout << "couldn't send packet" << std::endl;
						std::cout << "sending u instruction" << std::endl;
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
						game_packet gamePacket;
						gamePacket.type = PLAYER_MOVE_REQUEST;
						ins = "d" + player_number;
						gamePacket.player_move = ins;
						packet << gamePacket;
						if (socket.send(packet) != sf::Socket::Done)
							std::cout << "couldn't send packet" << std::endl;
						std::cout << "sending d instruction" << std::endl;
					}
					break;
				default:
					break;
				}
			}
		}
	}
	
	return 0;
}