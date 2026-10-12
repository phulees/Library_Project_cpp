#include "user.h"

#include <iostream>

User::User(int userId, const std::string& name,
           const std::string& phoneNumber, const std::string& email)
    : userId(userId), name(name), phoneNumber(phoneNumber), email(email)
{
}

int User::getUserId() const { return userId; }
const std::string& User::getName() const { return name; }
const std::string& User::getPhoneNumber() const { return phoneNumber; }
const std::string& User::getEmail() const { return email; }
const std::vector<int>& User::getBorrowedBookIds() const { return borrowedBookIds; }

bool User::isValid() const
{
    return userId > 0 && !name.empty();
}

bool User::canBorrowBook(int bookId) const
{
    if (bookId <= 0 || borrowedBookIds.size() >= MAX_BORROWED_BOOKS) return false;
    for (int borrowedId : borrowedBookIds) {
        if (borrowedId == bookId) return false;
    }
    return true;
}

bool User::borrowBook(int bookId)
{
    if (!canBorrowBook(bookId)) return false;
    borrowedBookIds.push_back(bookId);
    return true;
}

bool User::returnBook(int bookId)
{
    for (std::vector<int>::iterator it = borrowedBookIds.begin();
         it != borrowedBookIds.end(); ++it) {
        if (*it == bookId) {
            borrowedBookIds.erase(it);
            return true;
        }
    }
    return false;
}

void User::displayInfo() const
{
    std::cout << "Ma doc gia: " << userId << '\n'
              << "Ho ten: " << name << '\n'
              << "So dien thoai: " << phoneNumber << '\n'
              << "Email: " << email << '\n'
              << "So sach dang muon: " << borrowedBookIds.size()
              << "/" << MAX_BORROWED_BOOKS << '\n';

    if (borrowedBookIds.empty()) {
        std::cout << "Sach dang muon: Khong co\n";
    } else {
        std::cout << "Ma sach dang muon: ";
        for (std::size_t i = 0; i < borrowedBookIds.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << borrowedBookIds[i];
        }
        std::cout << '\n';
    }
}
