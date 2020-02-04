#include "review.h"
#include <sstream>

using namespace std;

Review::Review() :  rating(0), username(), date(), reviewText()
    { }
Review::Review(int rate,
        const std::string& u,
        const std::string& d,
        const std::string& review_text) :

     rating(rate),
     username(u),
     date(d),
     reviewText(review_text)
	{ }

std::string Review::ReviewData(){
    std::stringstream ss;
    ss << rating << " " << username << " ";
    ss << date << " " << reviewText;
    return ss.str();
}
std::string Review::getDate(){
    return date;
}