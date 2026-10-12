#pragma once

#include <iostream>
#include <string>

class Book
{
private:
    int id;
    std::string title;
    std::string author;
    int year;
    int totalCopies;
    int availableCopies;

public:
    Book();
    Book(int id, const std::string& title, const std::string& author,
         int year, int totalCopies, int availableCopies);
    ~Book() = default;

    int getId() const;
    const std::string& getTitle() const;
    const std::string& getAuthor() const;
    int getYear() const;
    int getTotalCopies() const;
    int getAvailableCopies() const;

    bool setId(int id);
    bool setTitle(const std::string& title);
    bool setAuthor(const std::string& author);
    bool setYear(int year);
    bool setTotalCopies(int totalCopies);
    bool setAvailableCopies(int availableCopies);

    bool isValid() const;
    bool borrowBook();
    bool returnBook();
    void displayInfo() const;
};
