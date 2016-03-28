#include <iostream>
#include "databaseram.h"
#include <algorithm>

std::vector<Newsgroup*> DatabaseRAM::getNewsgroups() {
	return newsgroups;
}


std::string DatabaseRAM::listArticles() {
}

std::string DatabaseRAM::getArticle() {
}
//Namn, något idnummer
void DatabaseRAM::createNewsgroup(std::string title) {
	newsgroups.push_back(new Newsgroup(title, nbrOfNews++));
}
//Namn, id, author, text
void DatabaseRAM::createArticle(int groupid, std::string title, std::string author, std::string text) {
	for_each (newsgroups.begin(), newsgroups.end(), [groupid, title, author, text] (Newsgroup* news) { if (groupid == news->getId()) news->createArticle(title, author, text); } );
}

void DatabaseRAM::deleteArticle() {
}

void Newsgroup::createArticle(std::string title, std::string author, std::string text){
}

void DatabaseRAM::deleteNewsgroup(int id) {
	auto last = remove_if(newsgroups.begin(), newsgroups.end(), [id] (Newsgroup* news)->bool { if (id == news->getId()) { delete news; return true; } return false; } );
	newsgroups.erase(last, newsgroups.end());
}

DatabaseRAM::DatabaseRAM() {
	nbrOfNews = 0;
}

/*
int main() {

	DatabaseRAM db;
	db.createNewsgroup("First");
	db.createNewsgroup("Second");
	std::vector<Newsgroup*> groups = db.getNewsgroups();
	for_each(groups.begin(), groups.end(), [] (Newsgroup* news){ std::cout << news->getTitle() << " " << news->getId()  << std::endl;});
	db.deleteNewsgroup(0);	
	db.deleteNewsgroup(1);	
	return 0;
	
}
*/
