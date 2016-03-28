#include <iostream>
#include "messagehandler.h"
#include "protocol.h"
#include <string>

MessageHandler::MessageHandler(){}

void MessageHandler::decode(std::shared_ptr<Connection>& con){
	cmd = con->read();
	std::cout << (int)cmd << std::endl;
	std::string answer;
	std::string title;


	switch((int)cmd){
		case Protocol::COM_LIST_NG : // 1
			std::cout << "list new group" << std::endl;
			answer.append(1, (unsigned char)Protocol::ANS_LIST_NG); 
			//answer += " ";
			answer.append(1, (unsigned char)Protocol::ZERO); 	
			answer.append(1, (unsigned char)Protocol::ANS_END); 
			//ser i wireshark 1,20,0,27,8 men 0 och 27 skickas i samma paket
			sendResponse(answer, con);
			break;

		case Protocol::COM_CREATE_NG : //2
			std::cout << "create new group" << std::endl;

			cmd = con->read(); // 40 for string
			findString(con);
			title = fromFindString;
			std::cout << "title: " << title << std::endl;

			//databas stuff

			//ser i wireshark då "anna" skickats 2,40,0,0,0,4,97,110,110,97,8

			break;

		case Protocol::COM_DELETE_NG : // 3
			std::cout << "delete newsgroup" << std::endl;
			cmd = con->read(); // 41 for number

			int number = findNumber(con);

			//databas stuff
			break;

		case Protocol::COM_LIST_ART : //4
			std::cout << "list articles" << std::endl;
			cmd = con->read(); // 41 for number

			int number = findNumber(con);
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

	void MessageHandler::findString(std::shared_ptr<Connection>& con){
			fromFindString = "";
			int length = findNumber(con);

			while (length != 0){
				-- length;
				cmd = con->read();
				fromFindString += cmd;
			}
	}

	int MessageHandler::findNumber(std::shared_ptr<Connection>& con){
			int length = 0;
			//ta ut längden
			cmd = con->read();
			std::cout << "cmd : " << (int)cmd << std::endl;
			length = length ^ (cmd << 24);
			cmd = con->read();
			std::cout << "cmd : " << (int)cmd << std::endl;
			length = length ^ (cmd << 16);
			cmd = con->read();
			std::cout << "cmd : " << (int)cmd << std::endl;
			length = length ^ (cmd << 8);
			cmd = con->read();
			std::cout << "cmd : " << (int)cmd << std::endl;
			length = length ^ cmd;
			return length;
	}


	void MessageHandler::sendResponse(std::string answer, std::shared_ptr<Connection>& con){
					//skicka svaret
			for (char c : answer){
				con->write(c);
			}
	}

