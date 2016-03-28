#include "connection.h"
#include "myserver.h"

class MessageHandler{
public:
	MessageHandler();
	void decode(std::shared_ptr<Connection>& con);

private:
	void sendResponse(std::string answer, std::shared_ptr<Connection>& con);
};