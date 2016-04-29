#ifndef DBFILE_H
#define DBFILE_H

#include "database.h"


class NewsFile : public News {
	public:
		NewsFile(std::string folder, int id) : folder(folder), id(id)  {
		nextId = 0;
		}
		~NewsFile() {};
		std::string getTitle() { return folder; }
		int getId() { return id; }
		void createArticle(std::string title, std::string author, std::string text);
		bool deleteArticle(int id); 
		std::vector<Article*> getArticles();
		Article* getArticle(int id);
	private:
		unsigned long findNextId();
		std::string folder;
		int id;
		int inodeNbr;
		unsigned int nextId;
};

class DBFile : public Database {
	public:
		DBFile();
		std::vector<News*> getNewsgroups();
		bool createNewsgroup(std::string title);
		bool deleteNewsgroup(int id);
		News* getNewsgroup(int id);
		News* getNewsgroup(std::string title);
	private:
		unsigned int inodeNbr;
		unsigned int nextId;
};

#endif
