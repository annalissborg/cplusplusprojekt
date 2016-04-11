#include "client.h"
#include <iostream>
#include "connection.h"
#include "protocol.h"
#include "messagehandler.h"

bool executeCommand(std::istream& input, std::shared_ptr<Connection>);
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
	while(true) {
		executeCommand(std::cin, con);
		input = "";
	}


	return 0;
}


bool executeCommand(std::istream& input, std::shared_ptr<Connection> con) {
	MessageHandler message;
	std::string command = "";
	input >> command;
	std::cout << std::endl;
	if(command == "group-list") {
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
