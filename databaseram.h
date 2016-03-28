#include "database.h"
#include <vector>

#ifndef DATABASERAM_H
#define DATABASERAM_H

class Newsgroup;
class DatabaseRAM : public Database {
	public:
		DatabaseRAM();
		std::vector<Newsgroup*> getNewsgroups();
		std::string listArticles();
		std::string getArticle();
		bool createNewsgroup(std::string title);
		void deleteNewsgroup(int id);
		void createArticle(int groupid, std::string title, std::string author, std::string text);
		void deleteArticle();
	private:
		std::vector<Newsgroup*> newsgroups;
		int nbrOfNews;
};

class Newsgroup {
	public:
		Newsgroup(std::string title, int id) : title(title), groupid(id) {}
		~Newsgroup() {};
		std::string getTitle() { return title; }
		int getId() { return groupid; }
		void createArticle(std::string title, std::string author, std::string text);
	private:
		std::string title;
		int groupid;
};

#endif
