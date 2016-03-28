#ifndef MYSERVER_H
#define MYSERVER_H

#include "server.h"


class MyServer : public Server{
public:
	MyServer(int port);

private:
	//snacka med databasen
};

#endif