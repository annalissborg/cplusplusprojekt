#include <vector>

#ifndef DATABASE_H
#define DATABASE_H

class Newsgroup;
class Database {
	public:
		virtual std::vector<Newsgroup*> getNewsgroups() = 0;
		virtual void createNewsgroup(std::string title) = 0;
		virtual void deleteNewsgroup(int id) = 0;
		virtual std::string listArticles() = 0;
		virtual void createArticle(int groupid, std::string title, std::string author, std::string text)= 0;
		virtual void deleteArticle() = 0;
		virtual std::string getArticle() = 0;
};

#endif
