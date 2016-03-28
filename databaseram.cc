#include <iostream>
#include "databaseram.h"
#include <algorithm>

std::vector<News*> DatabaseRAM::getNewsgroups() {
	return newsgroups;
}


std::string DatabaseRAM::listArticles() {
	return NULL;
}

std::string DatabaseRAM::getArticle() {
	return NULL;
}
//Namn, något idnummer
bool DatabaseRAM::createNewsgroup(std::string title) {
	News* news = getNewsgroup(title);
	if (!news)
			return false;
	newsgroups.push_back(new Newsgroup(title, nbrOfNews++));

	return true;
}
//Namn, id, author, text
void DatabaseRAM::createArticle(int groupid, std::string title, std::string author, std::string text) {
	for_each (newsgroups.begin(), newsgroups.end(), [groupid, title, author, text] (News* news) { if (groupid == news->getId()) news->createArticle(title, author, text); } );
}

bool DatabaseRAM::deleteArticle(int id) {
	auto last = remove_if(newsgroups.begin(), newsgroups.end(), [id] (News* news)->bool { if (id == news->getId()) { delete news; return true; } return false; } );
	newsgroups.erase(last, newsgroups.end());
	return true;
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

void Newsgroup::createArticle(std::string title, std::string author, std::string text){
}

bool DatabaseRAM::deleteNewsgroup(int id) {
	auto size = newsgroups.size();
	auto last = remove_if(newsgroups.begin(), newsgroups.end(), [id] (News* news)->bool { if (id == news->getId()) { delete news; return true; } return false; } );
	newsgroups.erase(last, newsgroups.end());
	return size == newsgroups.size();
}

DatabaseRAM::DatabaseRAM() {
	nbrOfNews = 0;
}
/*
=======


>>>>>>> 7a847a440131fb1fb9d9455fb0c72ce3c94a7fd0
int main() {

	DatabaseRAM db;
	db.createNewsgroup("First");
	db.createNewsgroup("Second");
	std::vector<News*> groups = db.getNewsgroups();
	for_each(groups.begin(), groups.end(), [] (News* news){ std::cout << news->getTitle() << " " << news->getId()  << std::endl;});
	db.deleteNewsgroup(0);	
	db.deleteNewsgroup(1);	
	return 0;
	
}*/
