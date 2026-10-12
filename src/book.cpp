#include "book.h"

Book::Book()
    : id(0), year(0), totalCopies(0), availableCopies(0)
{
}

Book::Book(int id, const std::string& title, const std::string& author,
           int year, int totalCopies, int availableCopies)
    : id(id), title(title), author(author), year(year),
      totalCopies(totalCopies), availableCopies(availableCopies)
{
}

int Book::getId() const { return id; }
const std::string& Book::getTitle() const { return title; }
const std::string& Book::getAuthor() const { return author; }
int Book::getYear() const { return year; }
int Book::getTotalCopies() const { return totalCopies; }
int Book::getAvailableCopies() const { return availableCopies; }

bool Book::setId(int id)
{
    if (id <= 0) return false;
    this->id = id;
    return true;
}

bool Book::setTitle(const std::string& title)
{
    if (title.empty()) return false;
    this->title = title;
    return true;
}

bool Book::setAuthor(const std::string& author)
{
    if (author.empty()) return false;
    this->author = author;
    return true;
}

bool Book::setYear(int year)
{
    if (year < 1000) return false;
    this->year = year;
    return true;
}

bool Book::setTotalCopies(int totalCopies)
{
    if (totalCopies < 1 || totalCopies < availableCopies) return false;
    this->totalCopies = totalCopies;
    return true;
}

bool Book::setAvailableCopies(int availableCopies)
{
    if (availableCopies < 0 || availableCopies > totalCopies) return false;
    this->availableCopies = availableCopies;
    return true;
}

bool Book::isValid() const
{
    return id > 0 && !title.empty() && !author.empty() &&
           year >= 1000 && totalCopies > 0 &&
           availableCopies >= 0 && availableCopies <= totalCopies;
}

bool Book::borrowBook()
{
    if (availableCopies <= 0) return false;
    --availableCopies;
    return true;
}

bool Book::returnBook()
{
    if (availableCopies >= totalCopies) return false;
    ++availableCopies;
    return true;
}

void Book::displayInfo() const
{
    std::cout << "Ma sach: " << id << '\n'
              << "Ten sach: " << title << '\n'
              << "Tac gia: " << author << '\n'
              << "Nam xuat ban: " << year << '\n'
              << "Tong so ban: " << totalCopies << '\n'
              << "So ban con: " << availableCopies << '\n';
}
