#include "user.h"
#include "hash.h"
using namespace std;

User::User() : name_("unknown"), balance_(0.0), type_(1), password_(0)
{

}
User::User(std::string name, double balance, int type,
	unsigned long long password) :
    name_(name), balance_(balance), type_(type), password_(password)
{

}

User::~User()
{

}


std::string User::getName() const
{
    return name_;
}

double User::getBalance() const
{
    return balance_;
}

bool User::checkPassword(std::string password){
	unsigned long long hash;
	try{
		hash = hash_str(password);
		return hash == password_;
	}
	catch(LongPassword& e){
		return false;
	}
}

void User::deductAmount(double amt)
{
    balance_ -= amt;
}

void User::dump(std::ostream& os)
{
    os << name_ << " "  << fixed << setprecision(2) << balance_ << " ";
    os << type_ << " " << password_ << endl;
}
