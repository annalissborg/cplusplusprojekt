#include "connection.h"
#include "myserver.h"

#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

class MessageHandler{
public:
	MessageHandler();
	void sendInt(int answer,  std::shared_ptr<Connection>& con);
	void sendChar(char c,  std::shared_ptr<Connection>& con);
	void sendString(std::string answer,  std::shared_ptr<Connection>& con);

private:


};
#endif
