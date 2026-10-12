#pragma once

#include <string>

class Book
{
private:
    int id;
    std::string title;
    std::string author;
    int year;
    int totalCopies;    // Tổng số bản sao (bao gồm cả sách cho mượn), chỉ thay đổi khi nhập kho hoặc tiêu hủy
    int availableCopies;    // Số bản sao khả dụng / còn lại

public:
    Book();
    Book(int id, const std::string& title, const std::string& author,
         int year, int totalCopies, int availableCopies);
    ~Book();

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
};
