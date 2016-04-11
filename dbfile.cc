#include "dbfile.h"
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
#include "database.h"
#include "databaseram.h"
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

unsigned long NewsFile::findNextId() {
	DIR* dir = opendir(folder.c_str());
	std::vector<Article*> articles;
	struct dirent* read = readdir(dir);
	unsigned  long nid = -1;
	while (read != NULL) {
		if (read->d_name[0] != '.') {
			unsigned int nid;
			std::string name = folder;
			name += "/";
			name += read->d_name;
			std::ifstream file(name.c_str());
			std::string title, author, sid, line,text = "";
			std::getline(file, title);
			std::getline(file, author);
			std::getline(file, sid);
			nid = std::stol(sid);
			if (nid >= id)
				id = nid + 1;
		}
		read = readdir(dir);
	}
	if (nid == -1)
		nid = 0;
	
	closedir(dir);
	return id;
}

void NewsFile::createArticle(std::string title, std::string author, std::string text) {
	std::string name = folder + "/";
	
	struct timeval tp;
	gettimeofday(&tp, NULL);
	long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	std::stringstream ss;
	ss << ms;
	name += ss.str();
	unsigned  long id = findNextId();
	std::ofstream file(name);
	file << title << "\n" << author << "\n" << id << "\n";
	file.close();
}

bool NewsFile::deleteArticle(int id) {
	DIR* dir = opendir(folder.c_str());
	std::vector<Article*> articles;
	struct dirent* read = readdir(dir);
	while (read != NULL) {
		if (read->d_name[0] != '.') {
			unsigned long nid;
			std::string name = folder;
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
/*
 * DIR* dir = opendir(folder.c_str());
	if (dir == NULL)
		return false;
	struct dirent* read = readdir(dir);
	while (read != NULL) {
		if (id + inodeNbr  == read->d_ino) {
			std::string name = folder;
			name += "/";
			name += read->d_name;
			return remove(name.c_str()) == 0;
		}
			read = readdir(dir);
	}
	*/
	return false;
}

std::vector<Article*> NewsFile::getArticles() {
	DIR* dir = opendir(folder.c_str());
	std::vector<Article*> articles;
	struct dirent* read = readdir(dir);
	Article* tmp = nullptr;
	while (read != NULL) {
		if (read->d_name[0] != '.') {
			unsigned long nid;
			std::string name = folder;
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
	DIR* dir = opendir(folder.c_str());
	if (dir == NULL)
		return nullptr;
	struct dirent* read = readdir(dir);

	while (read != NULL) {
		if (read->d_name[0] != '.') {
			unsigned long nid;
			std::string name = folder;
			name += "/";
			name += read->d_name;
			std::ifstream file(name.c_str());
			std::string title, author, sid, line,text = "";
			std::getline(file, title);
			std::getline(file, author);
			std::getline(file, sid);
			nid = std::stol(sid);
			if (id == nid) {
					while (std::getline(file, line)) {
				text += line + "\n";
				}
				text = text.substr(0, text.size()-1);
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
		dir = opendir("news");
	}
	struct dirent* read = readdir(dir);
		//Getting the inode number of the folder
	while (read != NULL) {
		if (strcmp(".",  read->d_name) == 0) {
			inodeNbr = read->d_ino;
		}
		read = readdir(dir);
	}
	closedir(dir);
}

std::vector<News*> DBFile::getNewsgroups() {
	std::vector<News*> news;
	DIR* dir = opendir("news");
	struct dirent* read = readdir(dir);
	int folder = 0;
	
	while (read != NULL) {
		if (read->d_name[0] != '.') {
			std::string name(read->d_name);
			news.push_back(new Newsgroup(name, read->d_ino - inodeNbr));
		}
		read = readdir(dir);
	}
	closedir(dir);
	return news;
}

bool DBFile::createNewsgroup(std::string title) {
	std::string name = "news/";
	name += title;
	DIR* dir = opendir(name.c_str());
	if (dir == NULL) {
		closedir(dir);
		int createDir = mkdir(name.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
		return createDir == 0;
	}
	closedir(dir);
	return false;
}

bool DBFile::deleteNewsgroup(int id) {
	DIR* dir = opendir("news");
	if (dir == NULL)
		return false;
	struct dirent* read = readdir(dir);
	while (read != NULL) {
		if (read->d_ino == id + inodeNbr) {
			std::string name = "news/";
			name += read->d_name;
			DIR* sub = opendir(name.c_str()); 				//To remove all files inside
			read = readdir(sub);
			while (read != NULL) {
				if (read->d_name[0] != '.') {
					std::string fileName = name + "/";
					fileName += read->d_name;
					remove(fileName.c_str());

				}
				read = readdir(sub);
			}
			closedir(sub);
			return remove (name.c_str()) == 0;
		}
		read = readdir(dir);
	}
	closedir(dir);
	return false;
}

News* DBFile::getNewsgroup(int id) {
	DIR* dir = opendir("news");
	if (dir == NULL)
		return nullptr;
	struct dirent* read = readdir(dir);
	while (read != NULL) {
		if (read->d_ino == id + inodeNbr) {
			std::string name = "news/";
			name += read->d_name;
			closedir(dir);
			return (new NewsFile(name, id, id + inodeNbr));
		}
		read = readdir(dir);
	}
	closedir(dir);
	return nullptr;
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
