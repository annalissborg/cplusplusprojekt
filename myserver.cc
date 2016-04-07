#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include "databaseram.h"
#include "myserver.h"
#include "messagehandler.h"
#include "protocol.h"
#include "connectionclosedexception.h"


MyServer::MyServer(int port) : Server(port){
	database = new DatabaseRAM();
}




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
	std::cout << "server is running" << std::endl;
	while(true){
		 auto con = server.waitForActivity();
		if(con != nullptr){ // finns någon på denna connection
			server.decode(con);
		}else{ //skapa en ny connection
			con = std::make_shared<Connection>();
			server.registerConnection(con);
		}
	}
}

void MyServer::decode(const std::shared_ptr<Connection>& con){
	int status = 0;
	try {
		cmd = con->read();
	}
	catch(ConnectionClosedException& e) {
		deregisterConnection(con);
		status = -1;
	}
	if (status >= 0) {
	std::cout << (int)cmd << std::endl;
	std::string answer;
	std::string title;
	std::string author;
	std::string text;
	int number = 0;
	int number2 = 0;
	auto list = database->getNewsgroups();
	std::vector<Article*> listArt;
	bool success = false;
	News* newsGroup; 
	Article* art;


	switch((int)cmd){
		case Protocol::COM_LIST_NG : // 1
			std::cout << "list new group" << std::endl;

			number = list.size();

			//creating the answer
			message.sendChar((unsigned char) Protocol::ANS_LIST_NG, con); 
			message.sendChar((unsigned char) Protocol::PAR_NUM, con);
			message.sendInt(number, con);
			for_each(list.begin(), list.end(), [this, con] (News* news) { message.sendChar((unsigned char)Protocol::PAR_NUM, con); message.sendInt(news->getId(), con); message.sendChar((unsigned char)Protocol::PAR_STRING, con); message.sendInt(news->getTitle().size(), con); message.sendString(news->getTitle(), con); });
			message.sendChar((unsigned char) Protocol::ANS_END, con); 
			break;





		case Protocol::COM_CREATE_NG : //2
			answer = "";
			std::cout << "create new group" << std::endl;
			cmd = con->read(); // 40 for string
			findString(con);
			title = fromFindString;
			//std::cout << "title: " << title << std::endl;
			success = database->createNewsgroup(title);

			message.sendChar((unsigned char)Protocol::ANS_CREATE_NG, con);
			if (success){
				message.sendChar((unsigned char)Protocol::ANS_ACK, con);
			} else {
				message.sendChar((unsigned char)Protocol::ANS_NAK, con);
				message.sendChar((unsigned char)Protocol::ERR_NG_ALREADY_EXISTS, con);
			}
			message.sendChar((unsigned char)Protocol::ANS_END, con);
			break;





		case Protocol::COM_DELETE_NG : // 3
			std::cout << "delete newsgroup" << std::endl;
			cmd = con->read(); // 41 for number
			number = findNumber(con);
			std::cout << "number from findNumber is: " << number << std::endl;
			success = database->deleteNewsgroup(number);

			//create answer
			message.sendChar((unsigned char)Protocol::ANS_DELETE_NG, con);
			if(success){
				message.sendChar((unsigned char)Protocol::ANS_ACK, con);
			}else{
				message.sendChar((unsigned char)Protocol::ANS_NAK, con);
				message.sendChar((unsigned char)Protocol::ERR_NG_DOES_NOT_EXIST, con);
			}
			message.sendChar((unsigned char)Protocol::ANS_END, con);
			break;




		case Protocol::COM_LIST_ART : //4
			std::cout << "list articles" << std::endl;
			cmd = con->read(); // 41 for number
			number = findNumber(con); // nummret till newsgroupen
			newsGroup = database->getNewsgroup(number);

			// creating the answer
			message.sendChar((unsigned char)Protocol::ANS_LIST_ART, con);

			if(newsGroup != nullptr){
				listArt = newsGroup->getArticles();
				message.sendChar((unsigned char)Protocol::ANS_ACK, con);
				message.sendChar((unsigned char)Protocol::PAR_NUM, con);
				message.sendInt(listArt.size(), con);
				for_each(listArt.begin(), listArt.end(), [this, con] (Article* art) { message.sendChar((unsigned char)Protocol::PAR_NUM, con); message.sendInt(art->getId(), con); message.sendChar((unsigned char)Protocol::PAR_STRING, con); message.sendInt(art->getTitle().size(), con); message.sendString(art->getTitle(), con); });
			}else{
				message.sendChar((unsigned char)Protocol::ANS_NAK, con);
				message.sendChar((unsigned char)Protocol::ERR_NG_DOES_NOT_EXIST, con);
			}
			message.sendChar((unsigned char)Protocol::ANS_END, con);
			break;		





		case Protocol::COM_CREATE_ART : //5
			std::cout << "create article" << std::endl;
			cmd = con->read(); // 41 for number
			number = findNumber(con);
				std::cout << "Creating article" << std::endl;

			cmd = con->read(); // 42 for string
			findString(con);
			title = fromFindString;
				std::cout << "Creating article" << std::endl;

			cmd = con->read(); // 42 for string
			findString(con);
			author = fromFindString;
				std::cout << "Creating article" << std::endl;

			cmd = con->read(); // 42 for string
			findString(con);
			text = fromFindString;
				std::cout << "Creating article" << std::endl;
			
			//create answer
			newsGroup = database->getNewsgroup(number);

			message.sendChar((unsigned char)Protocol::ANS_CREATE_ART, con);
			if(newsGroup != nullptr){
				std::cout << "Creating article" << std::endl;
				newsGroup->createArticle(title, author, text);
				std::cout << "Kommer förbi createArticle loopen" << std::endl;
				message.sendChar((unsigned char)Protocol::ANS_ACK, con);
			}else{
				std::cout << "Couldn't create article" << std::endl;
				message.sendChar((unsigned char)Protocol::ANS_NAK, con);
				message.sendChar((unsigned char)Protocol::ERR_NG_DOES_NOT_EXIST, con);
			}
			message.sendChar((unsigned char)Protocol::ANS_END, con);
			break;




		case Protocol::COM_DELETE_ART : 
			std::cout << "delete article" << std::endl;
			cmd = con->read(); // 41 for number
			number = findNumber(con); // group id

			cmd = con->read(); // 41 for number
			number2 = findNumber(con); // article id

			//create answer
			newsGroup = database->getNewsgroup(number);

			if(newsGroup != nullptr){
				success = newsGroup->deleteArticle(number2);
			}	

			message.sendChar((unsigned char)Protocol::ANS_DELETE_ART, con);

			if(success){
				message.sendChar((unsigned char)Protocol::ANS_ACK, con);
			}else{
				message.sendChar((unsigned char)Protocol::ANS_NAK, con);
				if(newsGroup != nullptr){
					message.sendChar((unsigned char)Protocol::ERR_ART_DOES_NOT_EXIST, con);
				}else{
					message.sendChar((unsigned char)Protocol::ERR_NG_DOES_NOT_EXIST, con);
				}
			}
			message.sendChar((unsigned char)Protocol::ANS_END, con);
			break;		





		case Protocol::COM_GET_ART : 
			std::cout << "get article" << std::endl;

			cmd = con->read(); // 41 for number
			number = findNumber(con); // group id

			cmd = con->read(); // 41 for number
			number2 = findNumber(con); // article id

			newsGroup = database->getNewsgroup(number);



			if(newsGroup != nullptr){
				std::cout << "Ska göra get article i server" << std::endl;
				art = newsGroup->getArticle(number2);
				std::cout << "Gjorde get article" << std::endl;
				if (art == nullptr) {
					
					message.sendChar((unsigned char)Protocol::ANS_NAK, con);
					message.sendChar((unsigned char)Protocol::ERR_ART_DOES_NOT_EXIST, con);
					message.sendChar((unsigned char)Protocol::ANS_END, con);
					return;
				}
				message.sendChar((unsigned char)Protocol::ANS_GET_ART, con);
				message.sendChar((unsigned char)Protocol::ANS_ACK, con);
				std::cout << "I server och skickar 1" << std::endl;
				message.sendChar((unsigned char)Protocol::PAR_STRING, con);
				std::cout << "I server och ska köra getTitle size" << std::endl;
				art->getTitle();
				std::cout << "I server och ska köra getTitle size" << std::endl;
				art->getTitle().size();
				std::cout << "I server och ska köra getTitle size" << std::endl;
				message.sendInt(art->getTitle().size(), con);
				std::cout << "I server och ska skicka titlen" << std::endl;
				message.sendString(art->getTitle(), con);
				std::cout << "I server och skickar 2" << std::endl;

				message.sendChar((unsigned char)Protocol::PAR_STRING, con);
				message.sendInt(art->getAuthor().size(), con);
				message.sendString(art->getAuthor(), con);
				std::cout << "I server och skickar 3" << std::endl;
	
				message.sendChar((unsigned char)Protocol::PAR_STRING, con);
				message.sendInt(art->getText().size(), con);
				message.sendString(art->getText(), con);
				std::cout << "I server och skickar 4" << std::endl;
				
			}else{
				message.sendChar((unsigned char)Protocol::ANS_NAK, con);
					message.sendChar((unsigned char)Protocol::ERR_NG_DOES_NOT_EXIST, con);
			}
			message.sendChar((unsigned char)Protocol::ANS_END, con);
			break;





		case Protocol::COM_END : 
			std::cout << "command end" << std::endl;
			//databas stuff ?
			break;
	}
	}
}

	void MyServer::findString(const std::shared_ptr<Connection>& con){
			fromFindString = "";
			int length = findNumber(con);

			while (length != 0){
				-- length;
				cmd = con->read();
				fromFindString += cmd;
			}
	}

	int MyServer::findNumber(const std::shared_ptr<Connection>& con){
			int length = 0;
			//ta ut längden
			cmd = con->read();
			length = length ^ (cmd << 24);
			cmd = con->read();
			length = length ^ (cmd << 16);
			cmd = con->read();
			length = length ^ (cmd << 8);
			cmd = con->read();
			length = length ^ cmd;
			return length;
	}




