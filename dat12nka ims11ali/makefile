#myserver.o: myserver.cc server.cc
#database.o: database.cc
#databaseram.o: databaseram.cc database.cc

all: serverram serverfile client

serverram: connection.o myserver.o  database.o messagehandler.o databaseram.o
		g++ -g -o serverram connection.o myserver.o server.o database.o messagehandler.o databaseram.o -std=c++11

serverfile: connection.o myserver2.o database.o messagehandler.o dbfile.o databaseram.o
		g++ -g -o serverfile connection.o myserver2.o server.o database.o messagehandler.o dbfile.o databaseram.o -std=c++11

client: client.o messagehandler.o connection.o
	g++ -g -o client connection.o client.o messagehandler.o

install:
	mv serverram bin/serverram
	mv serverfile bin/serverfile
	mv client bin/client

#myserver.o: myserver.cc server.cc
#	g++ -g -c -D_REENTRANT myserver.cc server.cc -std=c++11


messagehandler.o:
	g++ -g -c -D_REENTRANT src/messagehandler.cc -std=c++11

client.o:
	g++ -g -c -D_REENTRANT src/client.cc -std=c++11

database.o:
	g++ -g -c -D_REENTRANT src/database.cc -std=c++11

dbfile.o:
	g++ -g -c -D_REENTRANT src/dbfile.cc src/database.cc -std=c++11

databaseram.o:
	g++ -g -c -D_REENTRANT src/databaseram.cc src/database.cc -std=c++11


myserver2.o:
	g++ -g -c -D_REENTRANT src/myserver2.cc src/server.cc -std=c++11
	
myserver.o:
	g++ -g -c -D_REENTRANT src/myserver.cc src/server.cc -std=c++11

connection.o: 
	g++ -g -c -D_REENTRANT src/connection.cc -std=c++11
