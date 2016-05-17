#include <iostream>
#include <string>
#include "connection.h"
#include "protocol.h"
#include "messagehandler.h"

bool executeCommand(std::istream& input, std::shared_ptr<Connection>, int &grpNbr);
int findNumber(const std::shared_ptr<Connection>& con);
std::string findString(const std::shared_ptr<Connection>& con);

void printManual() {
	std::cout << std::endl;
	std::cout << "Type 1 to list the different newsgroups" << std::endl;	
	std::cout << "Type 2 <groupname> to create a new newsgroup" << std::endl;	
	std::cout << "Type 3 <group id>  to delete a  newsgroup" << std::endl;	
	std::cout << "Type 4 <group id> to list the articles in a group" << std::endl;	
	std::cout << "Type 5 <group id>  <Title> <enter> Author <enter> <text> to create an article" << std::endl;	
	std::cout << "Type 6 <group id> <article id> to delete an article" << std::endl;	
	std::cout << "Type 7 <groups id> <article id> to read an article from a group" << std::endl;	
	std::cout << "Type q to close the program" << std::endl;	
}


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
	printManual();
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
	//	input >> command;
	std::cout << std::endl;
	unsigned char response;
	std::string auhtor;
	std::string text;
	std::string title = ""; //To be used for various commands
	int number = -1; //To be used for various commands
	if(command == "1") {
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
	con->read();
	}
	else if (command == "2") {
		std::getline(input, title);
		message.sendChar((unsigned char) Protocol::COM_CREATE_NG, con);
		message.sendChar((unsigned char)Protocol::PAR_STRING, con);
		message.sendInt(title.size(), con); 
		message.sendString(title, con); 
		message.sendChar((unsigned char) Protocol::COM_END, con);


		con->read();	


		response = con->read();
		

		if(response == Protocol::ANS_ACK){
			std::cout << "The newsgroup named " << title << " has been created." << std::endl;
		} else{
			std::cout << "The newsgroup named " << title << " has NOT been created." << std::endl;
			response = con->read();
		}
		con->read();
	}
	else if (command == "3") {
		std::string tmp;
		input >> tmp;
		try {
			number = std::stoi(tmp);
		}
			catch(std::exception& e) {
			std::cout << "Du måste ange ett giltligt nummer" << std::endl;
			return false;
		}
		message.sendChar((unsigned int) Protocol::COM_DELETE_NG, con);
		message.sendChar((unsigned int) Protocol::PAR_NUM, con);
		message.sendInt(number, con);
		message.sendChar((unsigned int) Protocol::COM_END, con);

		con->read();
		response = con->read();
		if (response == Protocol::ANS_ACK)
			std::cout << "Newsgroup removed" << std::endl;
		else {
			std::cout << "Failed to remove newsgroup" << std::endl;
			con->read();
		}
		

	con->read();
	}

	else if (command == "4") {
			std::string tmp;
		input >> tmp;
		try {
			number = std::stoi(tmp);
		}
			catch(std::exception& e) {
			std::cout << "Du måste ange ett giltligt nummer" << std::endl;
			return false;
		}

		message.sendChar((unsigned char) Protocol::COM_LIST_ART, con);
		message.sendChar((unsigned char) Protocol::PAR_NUM, con);
		message.sendInt(number, con);
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
	else if (command == "5") {
		std::string tmp;
		input >> tmp;
		try {
			number = std::stoi(tmp);
		}
			catch(std::exception& e) {
			std::cout << "Du måste ange ett giltligt nummer" << std::endl;
			return false;
		}
			
			std::getline(input, title);
		std::cout << title << std::endl;
		std::getline(input, auhtor);
		std::cout << auhtor << std::endl;
		std::getline(input, text);
		std::cout << text << std::endl;

		message.sendChar((unsigned char) Protocol::COM_CREATE_ART, con);

		message.sendChar((unsigned char)Protocol::PAR_NUM, con);
		message.sendInt(number, con); 

		message.sendChar((unsigned char)Protocol::PAR_STRING, con);
		message.sendInt(title.size(), con); 
		message.sendString(title, con); 

		message.sendChar((unsigned char)Protocol::PAR_STRING, con);
		message.sendInt(auhtor.size(), con); 
		message.sendString(auhtor, con); 

		message.sendChar((unsigned char)Protocol::PAR_STRING, con);
		message.sendInt(text.size(), con); 
		message.sendString(text, con); 

		message.sendChar((unsigned char) Protocol::COM_END, con);


		con->read();	


		response = con->read();
		

		if(response == Protocol::ANS_ACK){
			std::cout << "The Article named " << title << " has been created." << std::endl;
		} else{
			std::cout << "The Article named " << title << " has NOT been created." << std::endl;
			response = con->read();
		}
		response = con->read();
	}
	else if (command == "6") {
		int group;
		std::string tmp1, tmp2;
		input >> tmp1;
		try {
			group = std::stoi(tmp1);
		}
			catch(std::exception& e) {
			std::cout << "Du måste ange ett giltligt nummer" << std::endl;
			return false;
		}
		input >> tmp2;
		try {
			number = std::stoi(tmp2);
		}
			catch(std::exception& e) {
			std::cout << "Du måste ange ett giltligt nummer" << std::endl;
			return false;
		}
		message.sendChar((unsigned int) Protocol::COM_DELETE_ART, con);
		message.sendChar((unsigned int) Protocol::PAR_NUM, con);
		message.sendInt(group, con);
		message.sendChar((unsigned int) Protocol::PAR_NUM, con);
		message.sendInt(number, con);
		message.sendChar((unsigned int) Protocol::COM_END, con);
		
		con->read();
		response = con->read();
		if (response == Protocol::ANS_ACK) {
			std::cout << "Successfully deleted the article" << std::endl;
		}
		else {
			std::cout << "Failed to delete the article" << std::endl;
				con->read();
		}
		con->read();
	}
	//Nogt er fucked
	else if (command == "7") {
		int group;
		std::string tmp1, tmp2;
		input >> tmp1;
		try {
			group = std::stoi(tmp1);
		}
			catch(std::exception& e) {
			std::cout << "Du måste ange ett giltligt nummer" << std::endl;
			return false;
		}
		input >> tmp2;
		try {
			number = std::stoi(tmp2);
		}
			catch(std::exception& e) {
			std::cout << "Du måste ange ett giltligt nummer" << std::endl;
			return false;
		}
			std::cout << "Reading article number: " << number << std::endl;
		message.sendChar((unsigned char) Protocol::COM_GET_ART, con);
		message.sendChar((unsigned char) Protocol::PAR_NUM, con);
		message.sendInt(group, con);
		message.sendChar((unsigned char) Protocol::PAR_NUM, con);
		message.sendInt(number, con);
		message.sendChar((unsigned char) Protocol::COM_END, con);

		con->read();
		unsigned char ack = con->read();
		if (ack == Protocol::ANS_ACK) {
			std::cout << "Title: ";
			con->read();
			text = findString(con);
			std::cout << text << std::endl;
			con->read();
			text = findString(con);
			std::cout << "Author: " << text << std::endl;
			con->read();
			text = findString(con);
			std::cout << std::endl << text << std::endl;
		}
		else {
			con->read();
			std::cout << "Failed to fetch article" << std::endl;
		}
		con->read();
	}
	else if (command == "q") {
		exit(0);
	}
	else {
		std::string remove;
		std::getline(input, remove);
		//printManual();
	}
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
