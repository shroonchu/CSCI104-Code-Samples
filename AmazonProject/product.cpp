#include <sstream>
#include <iomanip>
#include "product.h"
#include "review.h"

using namespace std;

Product::Product(const std::string category, const std::string name, 
    double price, int qty) :
    name_(name),
    price_(price),
    qty_(qty),
    category_(category)
{

}

Product::~Product()
{
    //delete the reviews
    for(std::vector<Review*>::iterator it=reviews_.begin(); it!=reviews_.end();
        it++){
        delete *it;
    }
}


double Product::getPrice() const
{
    return price_;
}

std::string Product::getName() const
{
    return name_;
}

void Product::subtractQty(int num)
{
    qty_ -= num;
}

int Product::getQty() const
{
    return qty_;
}

/**
 * default implementation...can be overriden in a future
 * assignment
 */
bool Product::isMatch(std::vector<std::string>& searchTerms) const
{
    return false;
}

void Product::dump(std::ostream& os) const
{
    os << category_ << "\n" << name_ << "\n" << price_;
    os << "\n" << qty_ << endl;
}

void Product::addReview(int rating,
    const std::string& username,
    const std::string& date,
    const std::string& review_text){
    Review* rev = new Review(rating, username, date, review_text);
    reviews_.push_back(rev);
}

void Product::dumpReviews(std::ostream& os) const{
    for(std::vector<Review*>::const_iterator it=reviews_.cbegin(); 
        it!=reviews_.cend(); it++){
        os << name_ << std::endl;
        os << (*it)->ReviewData();
        os << std::endl;
    }
}

double Product::getAvgRate() const{
    int num_rate = reviews_.size();
    if(num_rate==0){
        return 0;
    }
    double sum_rate = 0;
    for(std::vector<Review*>::const_iterator it=reviews_.cbegin(); 
        it!=reviews_.cend(); it++){
        sum_rate += (*it)->rating;
    }
    return sum_rate/num_rate;
}

std::vector<Review*> Product::getReviews() const{
    vector<Review*> revs = reviews_;
    return revs;
}