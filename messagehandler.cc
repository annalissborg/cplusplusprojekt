#include <iostream>
#include "messagehandler.h"
#include "protocol.h"
#include <string>

MessageHandler::MessageHandler(){}

	void MessageHandler::sendString(std::string answer,  const std::shared_ptr<Connection>& con){
					//skicka svaret
			for (char c : answer){
				con->write(c);
			}
	}

	void MessageHandler::sendChar(char c,  const std::shared_ptr<Connection>& con){
					//skicka svaret
				con->write(c);
	}

		void MessageHandler::sendInt(int answer,  const std::shared_ptr<Connection>& con){
				con->write((answer >> 24) & 0xFF);
				con->write((answer >> 16) & 0xFF);
				con->write((answer >> 8) & 0xFF);
				con->write(answer & 0xFF);

					//skicka svaret
	}

