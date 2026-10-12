#include "book.h"
#include "library.h"
#include "user.h"
#include <cassert>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {
void testBookRejectsInvalidState() {
    bool threw = false;
    try { static_cast<void>(Book(0, "C++", "Author", 2020, 1)); }
    catch (const std::invalid_argument&) { threw = true; }
    assert(threw);
}
void testBorrowAndReturn() {
    Library library;
    assert(library.addBook(Book(1, "Modern C++", "A. Author", 2022, 1)));
    assert(library.addUser(User(1, "Reader")));
    assert(library.borrowBook(1, 1));
    const auto afterBorrow = library.findBook(1);
    assert(afterBorrow && afterBorrow->get().availableCopies() == 0);
    assert(!library.borrowBook(1, 1));
    assert(!library.borrowBook(1, 999));
    assert(library.validateInvariants());
    assert(library.returnBook(1, 1));
    const auto afterReturn = library.findBook(1);
    assert(afterReturn && afterReturn->get().availableCopies() == 1);
    assert(!library.returnBook(1, 1));
    assert(library.validateInvariants());
}
void testBorrowLimit() {
    Library library;
    assert(library.addUser(User(1, "Reader")));
    for (int id = 1; id <= 6; ++id)
        assert(library.addBook(Book(id, "Book " + std::to_string(id), "Author", 2020, 1)));
    for (int id = 1; id <= 5; ++id) assert(library.borrowBook(1, id));
    assert(!library.borrowBook(1, 6));
    assert(library.validateInvariants());
}
void testDuplicateIdsAreRejected() {
    Library library;
    assert(library.addBook(Book(1, "First", "Author", 2020, 1)));
    assert(!library.addBook(Book(1, "Duplicate", "Another Author", 2021, 2)));
    assert(library.addUser(User(1, "Reader")));
    assert(!library.addUser(User(1, "Duplicate Reader")));
}
void testPersistenceRoundTrip() {
    const auto path = std::filesystem::temp_directory_path() / "library-modern-cpp-test-data.txt";
    std::error_code ignored;
    std::filesystem::remove(path, ignored);
    Library original;
    assert(original.addBook(Book(1, "C++ \"Modern\"", "A. Author", 2022, 2)));
    assert(original.addBook(Book(2, "Second Book", "B. Author", 2020, 1)));
    assert(original.addUser(User(1, "Reader Name", "0123", "reader@example.com")));
    assert(original.borrowBook(1, 1));
    original.saveToFile(path);
    const Library restored = Library::loadFromFile(path);
    assert(restored.validateInvariants());
    const auto book = restored.findBook(1);
    assert(book && book->get().title() == "C++ \"Modern\"");
    assert(book->get().availableCopies() == 1);
    const auto user = restored.findUser(1);
    assert(user && user->get().borrowedBookIds().contains(1));
    std::filesystem::remove(path, ignored);
}
}
int main() {
    testBookRejectsInvalidState();
    testBorrowAndReturn();
    testBorrowLimit();
    testDuplicateIdsAreRejected();
    testPersistenceRoundTrip();
    std::cout << "All library tests passed.\n";
}
