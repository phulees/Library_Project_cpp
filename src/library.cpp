#include "library.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

Book* Library::findBookById(int bookId)
{
    for (Book& book : books) if (book.getId() == bookId) return &book;
    return nullptr;
}

const Book* Library::findBookById(int bookId) const
{
    for (const Book& book : books) if (book.getId() == bookId) return &book;
    return nullptr;
}

User* Library::findUserById(int userId)
{
    for (User& user : users) if (user.getUserId() == userId) return &user;
    return nullptr;
}

const User* Library::findUserById(int userId) const
{
    for (const User& user : users) if (user.getUserId() == userId) return &user;
    return nullptr;
}

bool Library::addBook(const Book& book)
{
    if (!book.isValid() || findBookById(book.getId()) != nullptr) return false;
    books.push_back(book);
    return true;
}

bool Library::addUser(const User& user)
{
    if (!user.isValid() || findUserById(user.getUserId()) != nullptr) return false;
    users.push_back(user);
    return true;
}

bool Library::borrowBook(int userId, int bookId)
{
    User* user = findUserById(userId);
    Book* book = findBookById(bookId);
    if (user == nullptr || book == nullptr) return false;
    if (!user->canBorrowBook(bookId) || book->getAvailableCopies() <= 0) return false;
    if (!book->borrowBook()) return false;
    if (!user->borrowBook(bookId)) {
        book->returnBook();
        return false;
    }
    return true;
}

bool Library::returnBook(int userId, int bookId)
{
    User* user = findUserById(userId);
    Book* book = findBookById(bookId);
    if (user == nullptr || book == nullptr) return false;
    if (!user->returnBook(bookId)) return false;
    if (!book->returnBook()) {
        user->borrowBook(bookId);
        return false;
    }
    return true;
}

void Library::displayAllBooks() const
{
    if (books.empty()) { std::cout << "Thu vien chua co sach.\n"; return; }
    std::cout << "\n===== DANH SACH SACH =====\n";
    for (const Book& book : books) { book.displayInfo(); std::cout << "--------------------------\n"; }
}

void Library::displayAllUsers() const
{
    if (users.empty()) { std::cout << "Chua co doc gia nao.\n"; return; }
    std::cout << "\n===== DANH SACH DOC GIA =====\n";
    for (const User& user : users) { user.displayInfo(); std::cout << "-----------------------------\n"; }
}

void Library::displayBooksByTitle(const std::string& keyword) const
{
    std::string needle = keyword;
    std::transform(needle.begin(), needle.end(), needle.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    bool found = false;
    for (const Book& book : books) {
        std::string title = book.getTitle();
        std::transform(title.begin(), title.end(), title.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        if (title.find(needle) != std::string::npos) { book.displayInfo(); std::cout << "--------------------------\n"; found = true; }
    }
    if (!found) std::cout << "Khong tim thay sach phu hop.\n";
}
