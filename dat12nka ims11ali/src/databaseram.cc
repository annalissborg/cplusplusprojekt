#include <iostream>
#include "databaseram.h"
#include <algorithm>

std::vector<News*> DatabaseRAM::getNewsgroups() {
	return newsgroups;
}


std::vector<Article*> Newsgroup::getArticles() {
	return articles;
}

Article* Newsgroup::getArticle(int id) {
	auto it = find_if(articles.begin(), articles.end(), [id] (Article* art) { return art->getId() == id; } );
	if (it != articles.end())
		return *it;
	return nullptr;
}


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
	articles.push_back(new Article(title, author, text, ++nbrOfArticles));
}


bool Newsgroup::deleteArticle(int id) {
	int size = articles.size();
	auto last = remove_if(articles.begin(), articles.end(), [id] (Article* art)->bool { if (id == art->getId()) {  return true; } return false; } );
	articles.erase(last, articles.end());
	return size != articles.size();
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
	auto it = find_if(newsgroups.begin(), newsgroups.end(), [id] (News* news)->bool { std::cout << news->getId(); return id == news->getId(); } );
	if (it != newsgroups.end()) {
		return *it;
	}
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



