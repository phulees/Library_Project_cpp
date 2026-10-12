#include "book.h"

#include <stdexcept>
#include <utility>

namespace
{
bool isBlank(const std::string& value)
{
    return value.find_first_not_of(" \t\r\n") == std::string::npos;
}
}

Book::Book(Id id, std::string title, std::string author, int year, int totalCopies)
    : Book(id, std::move(title), std::move(author), year, totalCopies, totalCopies)
{
}

Book::Book(Id id, std::string title, std::string author, int year,
           int totalCopies, int availableCopies)
    : id_(id),
      title_(std::move(title)),
      author_(std::move(author)),
      year_(year),
      totalCopies_(totalCopies),
      availableCopies_(availableCopies)
{
    if (id_ <= 0) throw std::invalid_argument("Book ID must be positive.");
    if (isBlank(title_)) throw std::invalid_argument("Book title cannot be blank.");
    if (isBlank(author_)) throw std::invalid_argument("Book author cannot be blank.");
    if (year_ < 1000) throw std::invalid_argument("Publication year must be at least 1000.");
    if (totalCopies_ <= 0) throw std::invalid_argument("Total copies must be positive.");
    if (availableCopies_ < 0 || availableCopies_ > totalCopies_) {
        throw std::invalid_argument("Available copies must be between zero and total copies.");
    }
}

Book::Id Book::id() const noexcept { return id_; }
const std::string& Book::title() const noexcept { return title_; }
const std::string& Book::author() const noexcept { return author_; }
int Book::year() const noexcept { return year_; }
int Book::totalCopies() const noexcept { return totalCopies_; }
int Book::availableCopies() const noexcept { return availableCopies_; }
int Book::borrowedCopies() const noexcept { return totalCopies_ - availableCopies_; }
bool Book::canBeBorrowed() const noexcept { return availableCopies_ > 0; }

bool Book::checkout() noexcept
{
    if (!canBeBorrowed()) return false;
    --availableCopies_;
    return true;
}

bool Book::checkin() noexcept
{
    if (availableCopies_ >= totalCopies_) return false;
    ++availableCopies_;
    return true;
}
