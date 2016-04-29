#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include "databaseram.h"
#include "dbfile.h"
#include "myserver.h"
#include "messagehandler.h"
#include "protocol.h"
#include "connectionclosedexception.h"


MyServer::MyServer(int port) : Server(port){
//	database = new DatabaseRAM();
	database = new DBFile();
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
			cmd = con->read(); // 40 for string
			findString(con);
			title = fromFindString;
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
			cmd = con->read(); // 41 for number
			number = findNumber(con);
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
			cmd = con->read(); // 41 for number
			number = findNumber(con);

			cmd = con->read(); // 42 for string
			findString(con);
			title = fromFindString;

			cmd = con->read(); // 42 for string
			findString(con);
			author = fromFindString;

			cmd = con->read(); // 42 for string
			findString(con);
			text = fromFindString;
			
			//create answer
			newsGroup = database->getNewsgroup(number);

			message.sendChar((unsigned char)Protocol::ANS_CREATE_ART, con);
			if(newsGroup != nullptr){
				newsGroup->createArticle(title, author, text);
				message.sendChar((unsigned char)Protocol::ANS_ACK, con);
			}else{
				message.sendChar((unsigned char)Protocol::ANS_NAK, con);
				message.sendChar((unsigned char)Protocol::ERR_NG_DOES_NOT_EXIST, con);
			}
			message.sendChar((unsigned char)Protocol::ANS_END, con);
			break;




		case Protocol::COM_DELETE_ART : 
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

			cmd = con->read(); // 41 for number
			number = findNumber(con); // group id

			cmd = con->read(); // 41 for number
			number2 = findNumber(con); // article id

			newsGroup = database->getNewsgroup(number);


			message.sendChar((unsigned char)Protocol::ANS_GET_ART, con);

			if(newsGroup != nullptr){
				art = newsGroup->getArticle(number2);

				if (art == nullptr) {
					
					message.sendChar((unsigned char)Protocol::ANS_NAK, con);
					message.sendChar((unsigned char)Protocol::ERR_ART_DOES_NOT_EXIST, con);
					message.sendChar((unsigned char)Protocol::ANS_END, con);
					return;
				}
				
				message.sendChar((unsigned char)Protocol::ANS_ACK, con);

				message.sendChar((unsigned char)Protocol::PAR_STRING, con);
				message.sendInt(art->getTitle().size(), con);
				message.sendString(art->getTitle(), con);

				message.sendChar((unsigned char)Protocol::PAR_STRING, con);
				message.sendInt(art->getAuthor().size(), con);
				message.sendString(art->getAuthor(), con);
	
				message.sendChar((unsigned char)Protocol::PAR_STRING, con);
				message.sendInt(art->getText().size(), con);
				message.sendString(art->getText(), con);
				
			}else{
				message.sendChar((unsigned char)Protocol::ANS_NAK, con);
					message.sendChar((unsigned char)Protocol::ERR_NG_DOES_NOT_EXIST, con);
			}
			message.sendChar((unsigned char)Protocol::ANS_END, con);
			break;





		case Protocol::COM_END : 
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




