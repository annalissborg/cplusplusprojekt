#include <vector>

#ifndef DATABASE_H
#define DATABASE_H


class Article {
};

class News {
	public:
		virtual std::string getTitle() = 0;
		virtual int getId() = 0;
		virtual void createArticle(std::string title, std::string author, std::string text) = 0;
		virtual bool deleteArticle(int id) = 0;;
};

class Database {
	public:
		virtual std::vector<News*> getNewsgroups() = 0;
		virtual bool createNewsgroup(std::string title) = 0;
		virtual bool deleteNewsgroup(int id) = 0;
		virtual std::string listArticles() = 0;
		virtual std::string getArticle() = 0;
		virtual News* getNewsgroup(int id) = 0;
		virtual News* getNewsgroup(std::string title) = 0;
};


#endif

