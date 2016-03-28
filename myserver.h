#ifndef MYSERVER_H
#define MYSERVER_H

#include "server.h"


class MyServer : public Server{
public:
	MyServer(int port);
	void answer (std::shared_ptr<Connection> con, std::string& answer);

private:
	//snacka med databasen
};

#endif