#pragma once

#include <string>

class Book final
{
public:
    using Id = int;

    Book(Id id, std::string title, std::string author, int year, int totalCopies);
    Book(Id id, std::string title, std::string author, int year,
         int totalCopies, int availableCopies);

    [[nodiscard]] Id id() const noexcept;
    [[nodiscard]] const std::string& title() const noexcept;
    [[nodiscard]] const std::string& author() const noexcept;
    [[nodiscard]] int year() const noexcept;
    [[nodiscard]] int totalCopies() const noexcept;
    [[nodiscard]] int availableCopies() const noexcept;
    [[nodiscard]] int borrowedCopies() const noexcept;
    [[nodiscard]] bool canBeBorrowed() const noexcept;
    [[nodiscard]] bool checkout() noexcept;
    [[nodiscard]] bool checkin() noexcept;

private:
    Id id_;
    std::string title_;
    std::string author_;
    int year_;
    int totalCopies_;
    int availableCopies_;
};
