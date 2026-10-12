# Modern C++ notes for this project

This guide explains why the advanced branch differs from the beginner version. You do not need to understand every technique before using the program.

## 1. Who owns the memory?

Library owns books and readers inside std::unordered_map. When a Library is destroyed, its containers destroy their entries automatically. Strings and sets also release their resources automatically.

This is RAII: resource lifetime follows object lifetime. No explicit delete is needed.

A raw pointer such as Book* does not say who owns the book. If one object deletes the target, other pointers can become dangling. Here the reader stores book IDs instead, recording a relationship without owning the Book.

## 2. Why unordered_map?

The key is a book or reader ID; the value is the object. A typical ID lookup is average-case O(1), while scanning a vector is O(n).

An unordered map does not maintain sorted order. The listing methods sort references by ID so the UI remains deterministic.

## 3. What do std::move and pass-by-value do?

A constructor accepts strings by value and moves them into members:

    Book(Book::Id id, std::string title, std::string author, ...);
    title_(std::move(title))

When the caller passes a temporary string, moving can avoid an extra deep copy. When the caller passes an lvalue, the parameter is copied first. This is a common “pass by value, then move” pattern for owned values.

std::move itself does not move bytes; it permits overload resolution to select an operation that can treat the argument as an rvalue.

## 4. Why optional<reference_wrapper<const Book>>?

A lookup has two outcomes: found or not found. std::optional represents the missing case explicitly:

    const auto result = library.findBook(42);
    if (result) {
        const Book& book = result->get();
    }

The wrapper does not own the book. It is valid only while the owning library and referenced entry remain alive. This project currently has no deletion operation, simplifying lifetime rules.

## 5. Why can a constructor throw?

A Book with a negative ID or impossible copy count should never exist. The constructor rejects invalid data, so callers do not need to repeatedly check whether every Book is valid.

This is a class invariant. The console catches std::invalid_argument for invalid user input. Loading invalid persisted data fails rather than silently accepting it.

## 6. Why does Library coordinate loans?

Borrowing changes two pieces of state: the book's available-copy count and the reader's set of borrowed IDs. If only one side changes, the data contradicts itself.

Library::borrowBook checks both sides and inserts the ID into the reader's set before decrementing stock. Insertion may allocate; if it throws, the book count has not changed. validateInvariants() independently checks that recorded loans equal the number of unavailable copies for each book.

## 7. Tests are executable examples

Run make test. Tests demonstrate invalid construction, duplicate IDs, borrow/return rules, the five-book limit, and save/load round trips.

## Suggested reading order

1. include/book.h and src/book.cpp: encapsulation and invariants.
2. include/user.h and src/user.cpp: unordered_set and restricted mutation.
3. include/library.h and src/library.cpp: value ownership, optional lookups and coordinated operations.
4. tests/library_tests.cpp: how behavior is verified.
5. src/main.cpp: input/output kept outside domain classes.
