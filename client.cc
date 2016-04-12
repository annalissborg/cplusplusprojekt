#include <iostream>
#include <string>
#include "connection.h"
#include "protocol.h"
#include "messagehandler.h"

bool executeCommand(std::istream& input, std::shared_ptr<Connection>, int &grpNbr);
int findNumber(const std::shared_ptr<Connection>& con);
std::string findString(const std::shared_ptr<Connection>& con);

int main(int argc, char* argv[]) {

	if (argc != 3) {
		std::cerr << "Usage: host port" << std::endl;
		exit(1);
	}

	int port = -1;
	try {
		port = std::stoi(argv[2]);
	} catch (std::exception& e) {
		std::cerr << "Invalid port" << std::endl;
		exit(1);
	}

	std::cout << "Connection to server.." << std::endl;
		auto con = std::make_shared<Connection>(argv[1], port);
	if (!con->isConnected()) {
		std::cerr << "Faild to connect to host" << std::endl;
		exit(1);
	}
	std::cout << "Connected to server" << std::endl;
	std::string input = "";
	int grpNbr = -1;
	while(true) {
		executeCommand(std::cin, con, grpNbr);
		input = "";
	}


	return 0;
}


bool executeCommand(std::istream& input, std::shared_ptr<Connection> con, int &grpNbr) {
	MessageHandler message;
	std::string command = "";
	input >> command;
	std::cout << std::endl;
	unsigned char response;
	std::string title = ""; //To be used for various commands
	int number = -1; //To be used for various commands
	if(command == "list-groups") {
		message.sendChar((unsigned char) Protocol::COM_LIST_NG, con);
		message.sendChar((unsigned char) Protocol::COM_END, con);
		
		con->read();
		con->read();
		int nbrOfGroups = findNumber(con);
		std::string name = "";
		for (int i = 0; i != nbrOfGroups; ++i) {
			con->read();
			int id = findNumber(con);
			con->read();
			name = findString(con);
			std::cout << id << ") " << name << std::endl;
		}
	}
	else if (command == "use") {
		input >> grpNbr;
	}
	else if (command == "list-articles") {
		message.sendChar((unsigned char) Protocol::COM_LIST_ART, con);
		message.sendChar((unsigned char) Protocol::PAR_NUM, con);
		message.sendInt(grpNbr, con);
		message.sendChar((unsigned char) Protocol::COM_END, con);

		con->read();
		unsigned char ack = con->read();
		if (ack == Protocol::ANS_ACK) {
			con->read();
			int nbr = findNumber(con);
			for(int i = 0; i < nbr; ++i) {
				con->read();
				int artNbr = findNumber(con);
				con->read();
				std::string name = findString(con);
				std::cout << artNbr << ") " << name << std::endl;
			}
		}
		else {
			std::cout << "Faild to list articles" << std::endl;
			con->read();
		}
		con->read();


	}
	else if (command == "read") {
		input >> number;
		std::cout << "Reading article number: " << number << std::endl;
		message.sendChar((unsigned char) Protocol::COM_GET_ART, con);
		message.sendChar((unsigned char) Protocol::PAR_NUM, con);
		message.sendInt(grpNbr, con);
		message.sendChar((unsigned char) Protocol::PAR_NUM, con);
		message.sendInt(number, con);
		message.sendChar((unsigned char) Protocol::COM_END, con);
	
		//read input
		//
		con->read();
		unsigned char ack = con->read();
		if (ack == Protocol::ANS_ACK) {
			std::cout << "Article found!" << std::endl;
			ack = con->read();
			while(ack == Protocol::PAR_STRING) {
				std::string text = findString(con);
				std::cout << text << std::endl;
				ack = con->read();
			}
		}
		else {
			std::cout << "Failed to read article" << std::endl;
			con->read();
		}
		con->read();
	}
	else if (command == "createng") {
		input >> title;
		std::cout << "titel: " << title << std::endl;
		message.sendChar((unsigned char) Protocol::COM_CREATE_NG, con);
		message.sendChar((unsigned char)Protocol::PAR_STRING, con);
		message.sendInt(title.size(), con); 
		message.sendString(title, con); 
		message.sendChar((unsigned char) Protocol::COM_END, con);

		std::cout << "innan read answer " << std::endl;

		con->read();	
		std::cout << "innan read response " << std::endl;

		response = findNumber(con);
		std::cout << "response:  " << response << std::endl;

		if(response == Protocol::ANS_ACK){
			std::cout << "The newsgroup named " << title << " has been created." << std::endl;
		} else{
			std::cout << "The newsgroup named " << title << " has NOT been created." << std::endl;

		}
	}
	else
		std::cerr << "Unknown command" << std::endl;
		
	return false;
}

int findNumber(const std::shared_ptr<Connection>& con){
	int length = 0;
	unsigned char cmd = 0;
	cmd = con->read();
	length = length ^ (cmd << 24);
	cmd = con->read();
	length = length ^ (cmd << 16);
	cmd = con->read();
	length = length ^ (cmd << 8);
	cmd = con->read();
	length = length ^ cmd;

	return length;
}

std::string findString(const std::shared_ptr<Connection>& con){
	std::string string = "";
	int length = findNumber(con);
	unsigned char cmd;
	while (length != 0){
		-- length;
		cmd = con->read();
		string += cmd;

	}
	return string;

}
