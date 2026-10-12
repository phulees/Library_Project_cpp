#pragma once
#include "book.h"
#include "user.h"
#include <filesystem>
#include <functional>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <vector>

class Library final {
public:
    using BookReference = std::reference_wrapper<const Book>;
    using UserReference = std::reference_wrapper<const User>;
    [[nodiscard]] bool addBook(Book book);
    [[nodiscard]] bool addUser(User user);
    [[nodiscard]] bool borrowBook(User::Id userId, Book::Id bookId);
    [[nodiscard]] bool returnBook(User::Id userId, Book::Id bookId);
    [[nodiscard]] std::optional<BookReference> findBook(Book::Id id) const noexcept;
    [[nodiscard]] std::optional<UserReference> findUser(User::Id id) const noexcept;
    [[nodiscard]] std::vector<BookReference> allBooks() const;
    [[nodiscard]] std::vector<UserReference> allUsers() const;
    [[nodiscard]] std::vector<BookReference> searchBooks(std::string_view keyword) const;
    void saveToFile(const std::filesystem::path& path) const;
    [[nodiscard]] static Library loadFromFile(const std::filesystem::path& path);
    [[nodiscard]] bool validateInvariants() const;
private:
    std::unordered_map<Book::Id, Book> books_;
    std::unordered_map<User::Id, User> users_;
};
