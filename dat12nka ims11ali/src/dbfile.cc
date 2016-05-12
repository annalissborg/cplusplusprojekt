#include "dbfile.h"
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
#include "database.h"
#include <algorithm>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sstream>
#include <iomanip>

	long DBFile::findNextId() {
			std::string name = "news/info";
			std::ifstream file(name.c_str());
			std::string line;
			std::getline(file, line);
			long nid = stol(line) +1;
		
		return nid;
	}
	
	void DBFile::saveInfo(std::vector<News*> news) {
		std::string name = "news/info";
		long id = nextId;
		std::ofstream file(name.c_str());
		file << id << "\n";
		for_each(news.begin(), news.end(), [&file] (News* n) {
				file << n->getId() << "=" << n->getTitle() << "\n";
			});
		file.close();
	}

	long NewsFile::findNextId() {
		std::string path = "news/" + folder;
		DIR* dir = opendir(path.c_str());
		std::vector<Article*> articles;
		struct dirent* read = readdir(dir);
		long nid = -1;
		if (read != NULL) {
				std::string name = path;
				name += "/";
				name += "info";
				std::ifstream file(name.c_str());
				std::string line;
				std::getline(file, line);
				nid = stol(line) +1;
			read = readdir(dir);
		}
		if (nid == -1)
			nid = 1;
		
		closedir(dir);
		return nid;
	}

	void NewsFile::incrementNextId() {
		long nid = findNextId();
		std::stringstream ss;
		ss << "news/" << folder;
		ss << "/" << "info";
		std::ofstream file(ss.str());
		file << nid ;	
		file.close();
	}

	

	void NewsFile::createArticle(std::string title, std::string author, std::string text) {
		std::string name = "news/" + folder + "/";
		timeval tp;
		gettimeofday(&tp, NULL);
		id = findNextId(); 

		 long id = findNextId();
		std::stringstream ss;
		ss << id;
		name += ss.str();
		incrementNextId();
		std::ofstream file(name);
		file << title << "\n" << author << "\n" << id << "\n" << text; 
		file.close();
	}

	bool NewsFile::deleteArticle(int id) {
		std::string path = "news/" + folder;
		DIR* dir = opendir(path.c_str());
		std::vector<Article*> articles;
		struct dirent* read = readdir(dir);
		while (read != NULL) {
			if (read->d_name[0] != '.' && strcmp(read->d_name, "info") != 0) {
				unsigned long nid;
				std::string name = path;
				name += "/";
				name += read->d_name;
				std::ifstream file(name.c_str());
				std::string title, author, sid, line,text = "";
				std::getline(file, title);
				std::getline(file, author);
				std::getline(file, sid);
				nid = std::stol(sid);
				if (id == nid) {
					file.close();
					remove(name.c_str());
					closedir(dir);
					return true;
				}
			}
			read = readdir(dir);
		}
		closedir(dir);

		return false;
	}

	std::vector<Article*> NewsFile::getArticles() {
		std::string path = "news/" + folder;
		DIR* dir = opendir(path.c_str());
		std::vector<Article*> articles;
		struct dirent* read = readdir(dir);
		Article* tmp = nullptr;
		while (read != NULL) {
			if (read->d_name[0] != '.' && strcmp(read->d_name, "info") != 0) {
				unsigned long nid;
				std::string name = "news/" + folder;
				name += "/";
				name += read->d_name;
				std::ifstream file(name.c_str());
				std::string title, author, sid, line,text = "";
				std::getline(file, title);
				std::getline(file, author);
				std::getline(file, sid);

				nid = std::stol(sid);
					articles.push_back(new Article(title, "", "", nid));	
			}
			read = readdir(dir);
		}
		closedir(dir);
		sort(articles.begin(), articles.end(), [](Article* a1, Article* a2) {
				return a1->getId() < a2->getId();}
				);
		for_each(articles.begin(), articles.end(), [](Article* art) { 
				});
		return articles;	
	}

	Article* NewsFile::getArticle(int id) {
		std::string path = "news/" + folder;
		std::string s = std::to_string(id);
		char const *pchar = s.c_str(); 
		DIR* dir = opendir(path.c_str());
		if (dir == NULL)
			return nullptr;
		struct dirent* read = readdir(dir);

		while (read != NULL) {
			if (strcmp(read->d_name, pchar) == 0) {
				unsigned long nid;
				std::string name = path;
				name += "/";
				name += read->d_name;
				std::ifstream file(name.c_str());
				std::string title, author, sid, line,text = "";
				std::getline(file, title);
				std::getline(file, author);
				std::getline(file, sid);
				nid = std::stol(sid);
				std::string buffer;
				if (id == nid) {
						char ch;
					while(file.get(ch)) {
					text.push_back(ch);
				}
				file.close();
				closedir(dir);
				return new Article(title, author, text, nid);
				}
			}
			read = readdir(dir);
		}

		closedir(dir);
		return nullptr;
	}

DBFile::DBFile() {
		DIR* dir = opendir("news");
		if (dir == NULL ) {
			if(mkdir("news", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) != 0) {
				exit(EXIT_FAILURE);
			}
			else {
				std::stringstream ss;
				ss << "news/info";
				std::ofstream file(ss.str());
				file << 0 ;	
				file.close();	
			}
	}
closedir(dir);
}

std::vector<News*> DBFile::getNewsgroups() {
	std::vector<News*> news;
	std::ifstream file("news/info");
	std::string line, sid;
	int id;
	std::getline(file, line);
		std::getline(file, line);
	while(!file.eof()) {
		size_t pos = 0;

		sid = line.substr(0, pos+1);
		id = stoi(sid);
		line = line.substr(pos+2, line.length() - pos -2);
		news.push_back(new NewsFile(line, id));
		std::getline(file, line);
	}
	return news;
}

bool DBFile::createNewsgroup(std::string title) {
	std::string name = "news/";
	name += title;
	long id = findNextId();
	nextId = id;
	std::vector<News*> news = getNewsgroups();
	DIR* dir = opendir(name.c_str());
	if (dir == NULL) {
		closedir(dir);
		int createDir = mkdir(name.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

		std::stringstream ss;
		ss << name;
		ss << "/" << "info";
		std::ofstream file(ss.str());
		file << "0";
		file.close();
		news.push_back(new NewsFile(title, id)); 		
		saveInfo(news);
		return createDir == 0;
	}

	closedir(dir);
	return false;
}

bool DBFile::deleteNewsgroup(int id) {
	nextId = findNextId() -1;
	std::vector<News*> news = getNewsgroups();
	for (int i = 0; i < news.size(); ++i) {
		if (news[i]->getId() == id ) {
			std::string path = "news/" + news[i]->getTitle();
			std::cout << "Slänger " << path << std::endl;
			news.erase(news.begin() + i);
			saveInfo(news);
			DIR* dir = opendir(path.c_str());
			std::vector<Article*> articles;
			struct dirent* read = readdir(dir);
			while (read != NULL) {
				if (read->d_name[0] != '.') {
				unsigned long nid;
				std::string name = path;
				name += "/";
				name += read->d_name;
				std::cout << "Removing file:" << name << std::endl;
				remove(name.c_str());
			}
			read = readdir(dir);
		}
		closedir(dir);		
			return remove(path.c_str()) == 0;
			}
	}
	
	return false;
}

News* DBFile::getNewsgroup(int id) {
	std::vector<News*> news = getNewsgroups();
	for (int i = 0; i < news.size(); ++i) {
		if (news[i]->getId() == id)
			return news[i];
	}
	return nullptr;
/*
	DIR* dir = opendir("news");
	if (dir == NULL)
		return nullptr;
	struct dirent* read = readdir(dir);
	while (read != NULL) {
		if (read->d_ino == id + inodeNbr) {
			std::string name = "news/";
			name += read->d_name;
			closedir(dir);
			return (new NewsFile(name, id));
		}
		read = readdir(dir);
	}
	closedir(dir);
	return nullptr; */
}

News* DBFile::getNewsgroup(std::string title) {
	std::string name = "news/" + title;
	DIR* dir = opendir(name.c_str());
	if (dir == NULL) {
		closedir(dir);
		return nullptr;
	}
	closedir(dir);

}
