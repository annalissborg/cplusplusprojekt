#ifndef MYSERVER_H
#define MYSERVER_H

#include "server.h"
#include "connection.h"
#include "database.h"
#include "databaseram.h"
#include "messagehandler.h"
class MessageHandler;

class MyServer : public Server{
public:
	MyServer(int port);
	void decode(const std::shared_ptr<Connection>& con);

private:
	void findString(const std::shared_ptr<Connection>& con);
	int findNumber(const std::shared_ptr<Connection>& con);
	std::string fromFindString;
	unsigned char cmd;
	Database* database;
	MessageHandler message;
};

#endif
