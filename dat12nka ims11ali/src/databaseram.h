#include "database.h"
#include <vector>

#ifndef DATABASERAM_H
#define DATABASERAM_H



class Newsgroup : public News{
	public:
		Newsgroup(std::string title, int id) : title(title), groupid(id) {}
		~Newsgroup();
		std::string getTitle() { return title; }
		int getId() { return groupid; }
		void createArticle(std::string title, std::string author, std::string text);
		bool deleteArticle(int id);
		std::vector<Article*> getArticles();
		Article* getArticle(int id);
	private:
		std::string title;
		int groupid;
		int nbrOfArticles;
		std::vector<Article*> articles;
};

class DatabaseRAM : public Database {
	public:
		DatabaseRAM();
		std::vector<News*> getNewsgroups();
		bool createNewsgroup(std::string title);
		bool deleteNewsgroup(int id);
		News* getNewsgroup(int id);
		News* getNewsgroup(std::string title);
	private:
		std::vector<News*> newsgroups;
		int nbrOfNews;
};



#endif
