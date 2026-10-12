#include "user.h"

#include <stdexcept>
#include <utility>

namespace
{
bool isBlank(const std::string& value)
{
    return value.find_first_not_of(" \t\r\n") == std::string::npos;
}
}

User::User(Id id, std::string name, std::string phoneNumber, std::string email)
    : id_(id),
      name_(std::move(name)),
      phoneNumber_(std::move(phoneNumber)),
      email_(std::move(email))
{
    if (id_ <= 0) throw std::invalid_argument("User ID must be positive.");
    if (isBlank(name_)) throw std::invalid_argument("User name cannot be blank.");
}

User::Id User::id() const noexcept { return id_; }
const std::string& User::name() const noexcept { return name_; }
const std::string& User::phoneNumber() const noexcept { return phoneNumber_; }
const std::string& User::email() const noexcept { return email_; }

const std::unordered_set<int>& User::borrowedBookIds() const noexcept
{
    return borrowedBookIds_;
}

bool User::hasBorrowedBook(int bookId) const noexcept
{
    return borrowedBookIds_.contains(bookId);
}

bool User::canBorrowBook(int bookId) const noexcept
{
    return bookId > 0
        && !hasBorrowedBook(bookId)
        && borrowedBookIds_.size() < maxBorrowedBooks;
}

bool User::recordBorrow(int bookId)
{
    if (!canBorrowBook(bookId)) return false;
    return borrowedBookIds_.insert(bookId).second;
}

bool User::recordReturn(int bookId) noexcept
{
    return borrowedBookIds_.erase(bookId) != 0;
}
