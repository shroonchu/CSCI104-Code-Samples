#include <string>
#include <set>
#include <vector>
#include <sstream>
#include <iomanip>
#include "movie.h"
#include "util.h"

 Movie::Movie(const std::string category, 
 	const std::string name, double price, int qty,
	const std::string genre, const std::string rating) :
	Product(category, name, price, qty),
	genre_(genre),
	rating_(rating)
{
}

Movie::~Movie(){}

std::set<std::string> Movie::keywords() const{
	//movies have the words in their name and genre as keywords
	std::string keys = name_ + " " + genre_;
	std::set<std::string> keywords = parseStringToWords(keys);
	return keywords;
}

std::string Movie::displayString() const{
	std::stringstream ss;
	ss << name_ << std::endl;
	ss << "Genre: " << genre_;
	ss << " Rating: " << rating_ << std::endl;
	ss << price_ << " " << qty_ << " left." << std::endl;
	//place average rating into ss
	ss << "Rating: " << std::setprecision(2) << std::fixed << getAvgRate();
	std:: string display = ss.str();
	return display;
}
	
void Movie::dump(std::ostream& os) const{
	os << category_ << std::endl;
	os << name_ << std::endl;
	os << price_ << std::endl;
	os << qty_ << std::endl;
	os << genre_ << std::endl;
	os << rating_ << std::endl;
}