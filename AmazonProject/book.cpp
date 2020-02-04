#include <string>
#include <set>
#include <vector>
#include <sstream>
#include <iomanip>
#include "book.h"
#include "util.h"

Book::Book(const std::string category, const std::string name, 
	double price, int qty, const std::string isbn, 
	const std::string author) :
	Product(category, name, price, qty),
	isbn_(isbn),
	author_(author){}

Book::~Book(){}

std::set<std::string> Book::keywords() const{
	//books use words from their name and author as keywords
	std::set<std::string> parsename = parseStringToWords(name_);
	std::set<std::string> parseauthor = parseStringToWords(author_);
	//combine words from name and author
	std::set<std::string> keywords = setUnion(parsename, parseauthor);
	//books also have their isbn as keywords
	keywords.insert(isbn_);
	return keywords;
}

std::string Book::displayString() const{
	std::stringstream ss;
	ss << name_ << std::endl;
	ss << "Author: " << author_;
	ss << " ISBN: " << isbn_ << std::endl;
	ss << price_ << " " << qty_ << " left." << std::endl;
	//place average rating into ss
	ss << "Rating: " << std::setprecision(2) << std::fixed << getAvgRate();
	std::string display = ss.str();
	return display;
}
	
void Book::dump(std::ostream& os) const{
	os << category_ << std::endl;
	os << name_ << std::endl;
	os << price_ << std::endl;
	os << qty_ << std::endl;
	os << isbn_ << std::endl;
	os << author_ << std::endl;
}
