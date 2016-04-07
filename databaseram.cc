#include <iostream>
#include "databaseram.h"
#include <algorithm>

std::vector<News*> DatabaseRAM::getNewsgroups() {
	return newsgroups;
}


std::vector<Article*> Newsgroup::listArticles() {
	return articles;
}



//Namn, något idnummer
bool DatabaseRAM::createNewsgroup(std::string title) {
	News* news = getNewsgroup(title);
	if (news != nullptr) {
		return false;
	}
	newsgroups.push_back(new Newsgroup(title, ++nbrOfNews));
	return true;
}
//Namn, id, author, text
void Newsgroup::createArticle(std::string title, std::string author, std::string text) {
	//for_each (newsgroups.begin(), newsgroups.end(), [groupid, title, author, text] (News* news) { if (groupid == news->getId()) news->createArticle(title, author, text); } );
}


bool Newsgroup::deleteArticle(int id) {
//	auto last = remove_if(newsgroups.begin(), newsgroups.end(), [id] (News* news)->bool { if (id == news->getId()) { delete news; return true; } return false; } );
//	newsgroups.erase(last, newsgroups.end());
	return true;
}

Newsgroup::~Newsgroup() {
}

News* DatabaseRAM::getNewsgroup(std::string title) {
	auto it = find_if(newsgroups.begin(), newsgroups.end(), [title] (News* news) { return title.compare(news->getTitle()) == 0; } );
	if (it != newsgroups.end())
		return *it;
	return nullptr;
}
News* DatabaseRAM::getNewsgroup(int id) {
	auto it = find_if(newsgroups.begin(), newsgroups.end(), [id] (News* news) { return id == news->getId(); } );
	if (it != newsgroups.end())
		return *it;
	return nullptr;
}

bool DatabaseRAM::deleteNewsgroup(int id) {
	auto size = newsgroups.size();
	auto last = remove_if(newsgroups.begin(), newsgroups.end(), [id] (News* news)->bool { return (id == news->getId()) ? true : false; } );
	newsgroups.erase(last, newsgroups.end());
	return size != newsgroups.size();
}

DatabaseRAM::DatabaseRAM() {
	nbrOfNews = 0;
}

News::~News() {}



/*
int main() {

	DatabaseRAM db;
	db.createNewsgroup("First");
	db.createNewsgroup("Second");
	db.deleteNewsgroup(1);
	std::vector<News*> groups = db.getNewsgroups();
	return 0;
	
}*/
