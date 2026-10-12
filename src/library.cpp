#include "library.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace {
std::string lowerAscii(std::string_view text) {
    std::string result;
    result.reserve(text.size());
    for (const unsigned char ch : text) result.push_back(static_cast<char>(std::tolower(ch)));
    return result;
}
}

bool Library::addBook(Book book) {
    const Book::Id id = book.id();
    return books_.emplace(id, std::move(book)).second;
}
bool Library::addUser(User user) {
    const User::Id id = user.id();
    return users_.emplace(id, std::move(user)).second;
}

bool Library::borrowBook(User::Id userId, Book::Id bookId) {
    const auto userIt = users_.find(userId);
    const auto bookIt = books_.find(bookId);
    if (userIt == users_.end() || bookIt == books_.end()) return false;
    User& user = userIt->second;
    Book& book = bookIt->second;
    if (!user.canBorrowBook(bookId) || !book.canBeBorrowed()) return false;

    // Set insertion may allocate. Do it before changing stock for exception safety.
    if (!user.recordBorrow(bookId)) return false;
    if (!book.checkout()) {
        static_cast<void>(user.recordReturn(bookId));
        return false;
    }
    return true;
}

bool Library::returnBook(User::Id userId, Book::Id bookId) {
    const auto userIt = users_.find(userId);
    const auto bookIt = books_.find(bookId);
    if (userIt == users_.end() || bookIt == books_.end()) return false;
    User& user = userIt->second;
    Book& book = bookIt->second;
    if (!user.hasBorrowedBook(bookId) || book.borrowedCopies() <= 0) return false;
    if (!book.checkin()) return false;
    if (!user.recordReturn(bookId)) {
        static_cast<void>(book.checkout());
        return false;
    }
    return true;
}

std::optional<Library::BookReference> Library::findBook(Book::Id id) const noexcept {
    const auto it = books_.find(id);
    if (it == books_.end()) return std::nullopt;
    return std::cref(it->second);
}
std::optional<Library::UserReference> Library::findUser(User::Id id) const noexcept {
    const auto it = users_.find(id);
    if (it == users_.end()) return std::nullopt;
    return std::cref(it->second);
}

std::vector<Library::BookReference> Library::allBooks() const {
    std::vector<BookReference> result;
    result.reserve(books_.size());
    for (const auto& [id, book] : books_) {
        static_cast<void>(id);
        result.emplace_back(std::cref(book));
    }
    std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) {
        return a.get().id() < b.get().id();
    });
    return result;
}
std::vector<Library::UserReference> Library::allUsers() const {
    std::vector<UserReference> result;
    result.reserve(users_.size());
    for (const auto& [id, user] : users_) {
        static_cast<void>(id);
        result.emplace_back(std::cref(user));
    }
    std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) {
        return a.get().id() < b.get().id();
    });
    return result;
}
std::vector<Library::BookReference> Library::searchBooks(std::string_view keyword) const {
    const std::string needle = lowerAscii(keyword);
    std::vector<BookReference> result;
    for (const BookReference reference : allBooks()) {
        const Book& book = reference.get();
        if (lowerAscii(book.title()).find(needle) != std::string::npos)
            result.emplace_back(std::cref(book));
    }
    return result;
}

bool Library::validateInvariants() const {
    std::unordered_map<Book::Id, int> loanCounts;
    for (const auto& [userId, user] : users_) {
        static_cast<void>(userId);
        if (user.borrowedBookIds().size() > User::maxBorrowedBooks) return false;
        for (const int bookId : user.borrowedBookIds()) {
            if (!books_.contains(bookId)) return false;
            ++loanCounts[bookId];
        }
    }
    for (const auto& [bookId, book] : books_) {
        const auto it = loanCounts.find(bookId);
        const int recorded = it == loanCounts.end() ? 0 : it->second;
        if (book.borrowedCopies() != recorded) return false;
    }
    return true;
}

void Library::saveToFile(const std::filesystem::path& path) const {
    if (!validateInvariants())
        throw std::logic_error("Library invariants are broken; refusing to save inconsistent data.");
    std::ofstream output(path, std::ios::out | std::ios::trunc);
    if (!output) throw std::runtime_error("Cannot open data file for writing: " + path.string());

    for (const BookReference reference : allBooks()) {
        const Book& book = reference.get();
        output << "BOOK " << book.id() << ' ' << book.year() << ' '
               << book.totalCopies() << ' ' << book.availableCopies() << ' '
               << std::quoted(book.title()) << ' ' << std::quoted(book.author()) << '\n';
    }
    for (const UserReference reference : allUsers()) {
        const User& user = reference.get();
        std::vector<int> ids(user.borrowedBookIds().begin(), user.borrowedBookIds().end());
        std::sort(ids.begin(), ids.end());
        output << "USER " << user.id() << ' ' << std::quoted(user.name()) << ' '
               << std::quoted(user.phoneNumber()) << ' ' << std::quoted(user.email())
               << ' ' << ids.size();
        for (const int id : ids) output << ' ' << id;
        output << '\n';
    }
    output.flush();
    if (!output) throw std::runtime_error("Failed while writing data file: " + path.string());
}

Library Library::loadFromFile(const std::filesystem::path& path) {
    std::ifstream input(path);
    if (!input) throw std::runtime_error("Cannot open data file for reading: " + path.string());
    Library library;
    std::string type;
    while (input >> type) {
        if (type == "BOOK") {
            Book::Id id{};
            int year{}, total{}, available{};
            std::string title, author;
            if (!(input >> id >> year >> total >> available >> std::quoted(title) >> std::quoted(author)))
                throw std::runtime_error("Invalid BOOK record in data file.");
            Book book(id, std::move(title), std::move(author), year, total, available);
            if (!library.addBook(std::move(book))) throw std::runtime_error("Duplicate book ID in data file.");
        } else if (type == "USER") {
            User::Id id{};
            std::string name, phone, email;
            std::size_t count{};
            if (!(input >> id >> std::quoted(name) >> std::quoted(phone) >> std::quoted(email) >> count))
                throw std::runtime_error("Invalid USER record in data file.");
            if (count > User::maxBorrowedBooks)
                throw std::runtime_error("A user exceeds the maximum number of borrowed books.");
            User user(id, std::move(name), std::move(phone), std::move(email));
            for (std::size_t i = 0; i < count; ++i) {
                int bookId{};
                if (!(input >> bookId) || !user.recordBorrow(bookId))
                    throw std::runtime_error("Invalid or duplicate loan in data file.");
            }
            if (!library.addUser(std::move(user))) throw std::runtime_error("Duplicate user ID in data file.");
        } else {
            throw std::runtime_error("Unknown record type in data file: " + type);
        }
    }
    if (input.bad()) throw std::runtime_error("I/O error while reading data file.");
    if (!library.validateInvariants())
        throw std::runtime_error("Loaded data is inconsistent: loan records do not match copy counts.");
    return library;
}
