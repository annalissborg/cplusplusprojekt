#include "database.h"
#include <vector>
class Newsgroup;
class DatabaseRAM : public Database {
	public:
		DatabaseRAM();
		std::vector<Newsgroup*> getNewsgroups();
		std::string listArticles();
		std::string getArticle();
		void createNewsgroup(std::string title);
		void deleteNewsgroup();
		void createArticle();
		void deleteArticle();
	private:
		std::vector<Newsgroup*> newsgroups;
		int nbrOfNews;
};

class Newsgroup {
	public:
		Newsgroup(std::string title, int id) : title(title), groupid(id) {}
		std::string getTitle() { return title; }

	private:
		std::string title;
		int groupid;
};
