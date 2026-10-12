#pragma once

#include "book.h"
#include <vector>

class User
{
private:
    int userId;
    std::string name;
    std::string phoneNumber;
    std::string email;
    std::vector<Book*> listBookBorrow; // number book <= 5

public:
    User(int userId = 0, std::string name = "", std::string phoneNumber = "", std::string email = "");
    ~User();
};
