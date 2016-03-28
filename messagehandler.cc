#include <iostream>
#include "messagehandler.h"
#include "protocol.h"
#include <string>

MessageHandler::MessageHandler(){}

	void MessageHandler::sendString(std::string answer,  std::shared_ptr<Connection>& con){
					//skicka svaret
			for (char c : answer){
				con->write(c);
			}
	}

	void MessageHandler::sendChar(char c,  std::shared_ptr<Connection>& con){
					//skicka svaret
				con->write(c);
	}

		void MessageHandler::sendInt(int answer,  std::shared_ptr<Connection>& con){
					//skicka svaret
	}

