#include <vector>
class Newsgroup;
class Database {
	public:
		virtual std::vector<Newsgroup*> getNewsgroups() = 0;
		virtual void createNewsgroup(std::string title) = 0;
		virtual void deleteNewsgroup() = 0;
		virtual std::string listArticles() = 0;
		virtual void createArticle() = 0;
		virtual void deleteArticle() = 0;
		virtual std::string getArticle() = 0;
};
