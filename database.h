#include <vector>
#include <string>

#ifndef DATABASE_H
#define DATABASE_H


class Article {
	public:
		Article(std::string title, std::string author, std::string text, int id);
		~Article();
		std::string getTitle();
		std::string getAuthor();
		std::string getText();
		int getId();
	private:
		std::string title;
		std::string author;
		std::string text;
		int id;

};


class News {
	public:
		virtual ~News() = 0;
		virtual std::string getTitle() = 0;
		virtual int getId() = 0;
		virtual void createArticle(std::string title, std::string author, std::string text) = 0;
		virtual bool deleteArticle(int id) = 0;
		virtual std::vector<Article*> getArticles() = 0;
};


class Database {
	public:
		virtual std::vector<News*> getNewsgroups() = 0;
		virtual bool createNewsgroup(std::string title) = 0;
		virtual bool deleteNewsgroup(int id) = 0;
		virtual News* getNewsgroup(int id) = 0;
		virtual News* getNewsgroup(std::string title) = 0;
};


#endif

