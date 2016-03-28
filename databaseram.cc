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
void DatabaseRAM::createArticle() {
}

void DatabaseRAM::deleteArticle() {
}

void DatabaseRAM::deleteNewsgroup() {
}

DatabaseRAM::DatabaseRAM() {
	nbrOfNews = 0;
}

int main() {

	DatabaseRAM db;
	db.createNewsgroup("First");
	db.createNewsgroup("Second");
	std::vector<Newsgroup*> groups = db.getNewsgroups();
	for_each(groups.begin(), groups.end(), [] (Newsgroup* news){ std::cout << news->getTitle() << std::endl;});
	return 0;
	
}
