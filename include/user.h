#pragma once

#include <string>
#include <vector>

class User
{
private:
    int userId;
    std::string name;
    std::string phoneNumber;
    std::string email;
    std::vector<int> borrowedBookIds;
    static const std::size_t MAX_BORROWED_BOOKS = 5;

public:
    User(int userId = 0, const std::string& name = "",
         const std::string& phoneNumber = "", const std::string& email = "");

    int getUserId() const;
    const std::string& getName() const;
    const std::string& getPhoneNumber() const;
    const std::string& getEmail() const;
    const std::vector<int>& getBorrowedBookIds() const;

    bool isValid() const;
    bool canBorrowBook(int bookId) const;
    bool borrowBook(int bookId);
    bool returnBook(int bookId);
    void displayInfo() const;
};
