#include <iostream>
#include "messagehandler.h"
#include "protocol.h"
#include <string>

MessageHandler::MessageHandler(){}

void MessageHandler::decode(std::shared_ptr<Connection>& con){
	char cmd = con->read();
	std::cout << (int)cmd << std::endl;
	std::string answer;
	std::string title;
	unsigned int length= 0;


	switch((int)cmd){
		case Protocol::COM_LIST_NG : // 1
			std::cout << "list new group" << std::endl;
			answer.append(1, (unsigned char)Protocol::ANS_LIST_NG); 
			//answer += " ";
			answer.append(1, (unsigned char)Protocol::ZERO); 	
			answer.append(1, (unsigned char)Protocol::ANS_END); 

			//ser i wireshark 1,20,0,27,8 men 0 och 27 skickas i samma paket
			//sendResponse(answer, con);

			break;

		case Protocol::COM_CREATE_NG : //2
			std::cout << "create new group" << std::endl;


			cmd = con->read();
			length = length ^ (std::atoi(&cmd) << 24);
			cmd = con->read();
			length = length ^ (std::atoi(&cmd) << 16);
			cmd = con->read();
			length = length ^ (std::atoi(&cmd) << 8);
			cmd = con->read();
			length = length ^ std::atoi(&cmd);
			std::cout << "Length : " << length << std::endl;
			while (length != 0){
				-- length;
				cmd = con->read();
				title += cmd;
			}
			std::cout << "title: " << title << std::endl;

			//ser i wireshark då "anna" skickats 2,40,0,0,0,4,97,110,110,97,8

			break;

		case Protocol::COM_DELETE_NG : 
			std::cout << "delete newsgroup" << std::endl;
			break;

		case Protocol::COM_LIST_ART : 
			std::cout << "list articles" << std::endl;
			break;		

		case Protocol::COM_CREATE_ART : 
			std::cout << "create article" << std::endl;
			break;

		case Protocol::COM_DELETE_ART : 
			std::cout << "delete article" << std::endl;
			break;		

		case Protocol::COM_GET_ART : 
			std::cout << "get article" << std::endl;
			break;

		case Protocol::COM_END : 
			std::cout << "command end" << std::endl;
			break;
	}
}

	void MessageHandler::sendResponse(std::string answer, std::shared_ptr<Connection>& con){
					//skicka svaret
			for (char c : answer){
				con->write(c);
			}
	}