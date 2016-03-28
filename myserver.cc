#include <iostream>
#include <cstdlib>
#include "myserver.h"
#include "messagehandler.h"

MyServer::MyServer(int port) : Server(port){}




int main (int argc, char* argv[]){
	if(argc != 2){
		std::cerr << "No portnumber given" << std::endl;
		exit(1);
	}
	int port = -1;
	try{
		port = std::stoi(argv[1]);
	}catch (std::exception& e){
		std::cerr << "Port invalid" << std::endl;
		exit(1);
	}

	MyServer server(port);
	if(!server.isReady()){
		std::cerr << "Server not ready" << std::endl;
		exit(1);
	}
	MessageHandler message;
	std::cout << "server is running" << std::endl;
	while(true){
		auto con = server.waitForActivity();
		if(con != nullptr){ // finns någon på denna connection
			message.decode(con);
		}else{ //skapa en ny connection
			con = std::make_shared<Connection>();
			server.registerConnection(con);
		}




	}
}