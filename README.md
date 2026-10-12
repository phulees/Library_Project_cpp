# Library Project — Modern C++

A console-based library management system for studying modern C++ design: class invariants, ownership, exception safety, standard-library containers, persistence, and automated tests.

## Features
- Add books and readers; reject duplicate IDs.
- Search books by title and list records in predictable ID order.
- Borrow and return books, with a maximum of five distinct book IDs per reader.
- Persist and reload data from library-data.txt.
- Validate persisted data before accepting it.
- Automated tests and GitHub Actions CI.

## Build and run
Requirements: a C++20 compiler (recent GCC or Clang) and GNU Make.

    make
    make run
    make test

Remove generated files with make clean.

The first run seeds a small sample library. Successful changes are saved automatically; exiting also saves the current state. The data file is created in the current working directory.

## Design choices

### Value ownership and RAII
Library owns books and users by value in std::unordered_map. Book and User own their strings and containers through standard-library types. There is no manual new/delete and no raw pointer represents ownership.

Modern C++ does not mean using smart pointers everywhere. Use std::unique_ptr when a dynamically allocated object needs one clear owner, and std::shared_ptr only when shared lifetime ownership is required. This project does not need either for its book/user records.

### Encapsulation and invariants
- Constructors reject invalid IDs, blank required text and impossible copy counts.
- A book's available-copy count always remains between zero and its total.
- A reader tracks borrowed book IDs in std::unordered_set, preventing duplicate loans and enabling average O(1) membership checks.
- Only Library can modify a reader's loan ledger.
- Library::validateInvariants() checks that recorded loans match the number of borrowed copies.

### Modern standard-library tools
- C++20: unordered_map::contains, structured bindings and modern language support.
- Move semantics: strings are passed by value and moved into owned members.
- std::optional<std::reference_wrapper<const T>> represents a missing lookup without a sentinel object. The returned reference is non-owning and must not outlive its library entry.
- std::filesystem provides platform-aware paths.
- std::quoted supports text containing spaces, quotes and backslashes.
- std::from_chars parses integer input strictly.

### Error handling and persistence
Constructors throw std::invalid_argument if an object would start invalid. File and data-format failures throw exceptions and are reported by main. Loading rejects unknown records, duplicate IDs, invalid object data, missing book references and mismatched loan/copy counts.

The persistence format is a simple text file, not a database. Saving writes directly to the data file; this is useful for learning but is not a crash-safe transactional storage engine.

### Complexity
Book/user lookup and ID uniqueness checks use std::unordered_map, average-case O(1). Title search is O(n). Listing sorts output by ID to keep the display deterministic.

## Source layout
- include/: public class interfaces
- src/: implementations and console entry point
- tests/library_tests.cpp: automated behavior tests
- .github/workflows/ci.yml: CI build/test workflow
- Makefile

## Current limitations
- Search lowercases ASCII bytes; it is not full Vietnamese Unicode case folding.
- No due dates, fines, deletion/editing UI, authentication or database backend.
- This is a single-process console application, not a thread-safe concurrent service.
- Saving directly to a file means a crash during a write could leave partial data.
