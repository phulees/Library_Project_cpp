#include "book.h"

Book::Book()
    : id(0), year(0), totalCopies(0), availableCopies(0)
{
}

Book::Book(int id, const std::string& title, const std::string& author,
           int year, int totalCopies, int availableCopies)
    : id(id),
      title(title),
      author(author),
      year(year),
      totalCopies(totalCopies),
      availableCopies(availableCopies)
{
}

Book::~Book() = default;

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
    if (year <= 1000) return false; // Làm j có thư viện nào lưu sách sx năm 1000 đâu đk=))
    this->year = year;
    return true;
}

bool Book::setTotalCopies(int totalCopies)
{
    if (totalCopies <= 0) return false;
    this->totalCopies = totalCopies;
    return true;
}

bool Book::setAvailableCopies(int availableCopies)
{
    if (availableCopies < 0) return false;
    this->availableCopies = availableCopies;
    return true;
}
