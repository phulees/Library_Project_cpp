#include "user.h"

User::User(int userId, std::string name, std::string phoneNumber, std::string email)
    : userId(userId), name(name), phoneNumber(phoneNumber), email(email)
{
}

User::~User()
{
    for (Book*& b : listBookBorrow) {
        delete b;
        b = nullptr;
    }
}