#pragma once

#include <cstddef>
#include <string>
#include <unordered_set>

class Library;

class User final
{
public:
    using Id = int;
    static constexpr std::size_t maxBorrowedBooks = 5;

    User(Id id, std::string name, std::string phoneNumber = {},
         std::string email = {});

    [[nodiscard]] Id id() const noexcept;
    [[nodiscard]] const std::string& name() const noexcept;
    [[nodiscard]] const std::string& phoneNumber() const noexcept;
    [[nodiscard]] const std::string& email() const noexcept;
    [[nodiscard]] const std::unordered_set<int>& borrowedBookIds() const noexcept;
    [[nodiscard]] bool hasBorrowedBook(int bookId) const noexcept;
    [[nodiscard]] bool canBorrowBook(int bookId) const noexcept;

private:
    friend class Library;
    [[nodiscard]] bool recordBorrow(int bookId);
    [[nodiscard]] bool recordReturn(int bookId) noexcept;

    Id id_;
    std::string name_;
    std::string phoneNumber_;
    std::string email_;
    std::unordered_set<int> borrowedBookIds_;
};
