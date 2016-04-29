#include "connection.h"
#include "myserver.h"

#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

class MessageHandler{
public:
	MessageHandler();
	void sendInt(int answer,  const std::shared_ptr<Connection>& con);
	void sendChar(char c,  const std::shared_ptr<Connection>& con);
	void sendString(std::string answer,  const std::shared_ptr<Connection>& con);

private:


};
#endif
