#include "connection.h"
#include "myserver.h"

class MessageHandler{
public:
	MessageHandler();
	void decode(std::shared_ptr<Connection>& con, MyServer& server);

private:
};