#include <iostream>
#include "messagehandler.h"
#include "protocol.h"
#include <string>

MessageHandler::MessageHandler(){}

void MessageHandler::decode(std::shared_ptr<Connection>& con, MyServer& server){
	char cmd = con->read();
	std::cout << (int)cmd << std::endl;
	std::string answer;

	switch((int)cmd){
		case Protocol::COM_LIST_NG : 
			std::cout << "list new group" << std::endl;
			answer.append(1, (unsigned char)Protocol::ANS_LIST_NG);
			//answer += " ";
			answer += "0";
			//answer += " ";
			answer.append(1, (unsigned char)Protocol::ANS_END);
			std::cout << "sista bit är: " << (int)Protocol::ANS_END << std::endl;
			server.answer(con, answer);
			break;

		case Protocol::COM_CREATE_NG : 
			std::cout << "create new group" << std::endl;
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