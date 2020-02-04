#ifndef REVIEW_H
#define REVIEW_H
#include <string>

// NOTE: You may add any data members or functions you deem useful

struct Review
{
    Review();
    Review(int rate,
           const std::string& u,
           const std::string& d,
           const std::string& review_text);
    std::string ReviewData();
    std::string getDate();

    int rating;
    std::string username;
    std::string date;
    std::string reviewText;
};

#endif
