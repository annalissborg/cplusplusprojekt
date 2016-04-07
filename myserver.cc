#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include "databaseram.h"
#include "myserver.h"
#include "messagehandler.h"
#include "protocol.h"


MyServer::MyServer(int port) : Server(port){
	database = new DatabaseRAM();
}




int main (int argc, char* argv[]){
	if(argc != 2){
		std::cerr << "No portnumber given" << std::endl;
		exit(1);
	}
	int port = -1;
	try{
		port = std::stoi(argv[1]);
	}catch (std::exception& e){
		std::cerr << "Port invalid" << std::endl;
		exit(1);
	}

	MyServer server(port);
	if(!server.isReady()){
		std::cerr << "Server not ready" << std::endl;
		exit(1);
	}
	std::cout << "server is running" << std::endl;
	while(true){
		 auto con = server.waitForActivity();
		if(con != nullptr){ // finns någon på denna connection
			server.decode(con);
		}else{ //skapa en ny connection
			con = std::make_shared<Connection>();
			server.registerConnection(con);
		}
	}
}

void MyServer::decode(const std::shared_ptr<Connection>& con){
	cmd = con->read();
	//std::cout << (int)cmd << std::endl;
	std::string answer;
	std::string title;
	int number = 0;
	auto list = database->getNewsgroups();
	bool success = false;


	switch((int)cmd){
		case Protocol::COM_LIST_NG : // 1
			std::cout << "list new group" << std::endl;

			std::cout <<"nbr of elements in list: " << list.size() << std::endl;
			number = list.size();
			std::cout << "number is: " << number << std::endl;

			std::cout << list.size() << std::endl;
			//for_each(list.begin(), list.end(), [] (Newsgroup* news) { std::cout << "size: " << news->getTitle().size() << "Title: " << news->getTitle() << std::endl;  });


			//creating the answer
			message.sendChar((unsigned char) Protocol::ANS_LIST_NG, con); 
			message.sendChar((unsigned char) Protocol::PAR_NUM, con);
			message.sendInt(number, con);
			for_each(list.begin(), list.end(), [this, con] (News* news) { message.sendChar((unsigned char)Protocol::PAR_NUM, con); message.sendInt(news->getId(), con); message.sendChar((unsigned char)Protocol::PAR_STRING, con); message.sendInt(news->getTitle().size(), con); message.sendString(news->getTitle(), con); });
			message.sendChar((unsigned char) Protocol::ANS_END, con); 
			break;

		case Protocol::COM_CREATE_NG : //2
			answer = "";
			std::cout << "create new group" << std::endl;
			cmd = con->read(); // 40 for string
			findString(con);
			title = fromFindString;
			std::cout << "title: " << title << std::endl;
			success = database->createNewsgroup(title);

			message.sendChar((unsigned char)Protocol::ANS_CREATE_NG, con);
			if (success){
				message.sendChar((unsigned char)Protocol::ANS_ACK, con);
			} else {
				message.sendChar((unsigned char)Protocol::ANS_NAK, con);
				message.sendChar((unsigned char)Protocol::ERR_NG_ALREADY_EXISTS, con);
			}
			message.sendChar((unsigned char)Protocol::ANS_END, con);
			break;

		case Protocol::COM_DELETE_NG : // 3
			std::cout << "delete newsgroup" << std::endl;
			cmd = con->read(); // 41 for number
			number = findNumber(con);
			database->deleteNewsgroup(number);

			//svara
			break;

		case Protocol::COM_LIST_ART : //4
			std::cout << "list articles" << std::endl;
			cmd = con->read(); // 41 for number

			number = findNumber(con);
			//databas stuff
			break;		

		case Protocol::COM_CREATE_ART : //5
			std::cout << "create article" << std::endl;
			
			
			//databas stuff
			break;

		case Protocol::COM_DELETE_ART : 
			std::cout << "delete article" << std::endl;
			//databas stuff
			break;		

		case Protocol::COM_GET_ART : 
			std::cout << "get article" << std::endl;
			//databas stuff
			break;

		case Protocol::COM_END : 
			std::cout << "command end" << std::endl;
			//databas stuff
			break;
	}
}

	void MyServer::findString(const std::shared_ptr<Connection>& con){
			fromFindString = "";
			int length = findNumber(con);

			while (length != 0){
				-- length;
				cmd = con->read();
				fromFindString += cmd;
			}
	}

	int MyServer::findNumber(const std::shared_ptr<Connection>& con){
			int length = 0;
			//ta ut längden
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




