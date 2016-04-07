#include "database.h"
#include <string>

Article::Article  (std::string title, std::string author, std::string text, int id) : title(title), author(author), text(text), id(id) {
}

std::string Article::getText() {
	return text;
}

std::string Article::getAuthor() {
	return author;
}

std::string Article::getTitle() {
	return title;
}

int Article::getId() {
	return id;
}
