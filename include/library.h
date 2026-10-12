#pragma once

#include "book.h"
#include "user.h"

#include <string>
#include <vector>

class Library
{
private:
    std::vector<Book> books;
    std::vector<User> users;

    Book* findBookById(int bookId);
    const Book* findBookById(int bookId) const;
    User* findUserById(int userId);
    const User* findUserById(int userId) const;

public:
    bool addBook(const Book& book);
    bool addUser(const User& user);
    bool borrowBook(int userId, int bookId);
    bool returnBook(int userId, int bookId);

    void displayAllBooks() const;
    void displayAllUsers() const;
    void displayBooksByTitle(const std::string& keyword) const;
};
