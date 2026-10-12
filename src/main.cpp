#include "library.h"

#include <iostream>
#include <sstream>
#include <string>

namespace {
int readInt(const std::string& prompt)
{
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) return -1;
        std::stringstream input(line);
        int value;
        char extra;
        if (input >> value && !(input >> extra)) return value;
        std::cout << "Vui long nhap mot so nguyen hop le.\n";
    }
}

std::string readText(const std::string& prompt)
{
    std::cout << prompt;
    std::string value;
    std::getline(std::cin, value);
    return value;
}

void showMenu()
{
    std::cout << "\n========== QUAN LY THU VIEN ==========\n"
              << "1. Them sach\n2. Hien thi tat ca sach\n3. Tim sach theo ten\n"
              << "4. Them doc gia\n5. Hien thi doc gia\n6. Muon sach\n"
              << "7. Tra sach\n0. Thoat\n======================================\n";
}
}

int main()
{
    Library library;
    library.addBook(Book(1, "Lap trinh C++", "Nguyen Van A", 2022, 3, 3));
    library.addBook(Book(2, "Nhap mon OOP", "Tran Thi B", 2023, 2, 2));
    library.addUser(User(1, "Nguyen An", "0900000001", "an@example.com"));

    while (true) {
        showMenu();
        const int choice = readInt("Chon chuc nang: ");
        if (choice == 0) { std::cout << "Da thoat chuong trinh.\n"; break; }
        switch (choice) {
        case 1: {
            int id = readInt("Ma sach: ");
            std::string title = readText("Ten sach: ");
            std::string author = readText("Tac gia: ");
            int year = readInt("Nam xuat ban: ");
            int total = readInt("Tong so ban: ");
            if (library.addBook(Book(id, title, author, year, total, total))) std::cout << "Them sach thanh cong.\n";
            else std::cout << "Them sach that bai. Kiem tra du lieu hoac ma sach bi trung.\n";
            break;
        }
        case 2: library.displayAllBooks(); break;
        case 3: library.displayBooksByTitle(readText("Nhap ten sach can tim: ")); break;
        case 4: {
            int id = readInt("Ma doc gia: ");
            std::string name = readText("Ho ten: ");
            std::string phone = readText("So dien thoai: ");
            std::string email = readText("Email: ");
            if (library.addUser(User(id, name, phone, email))) std::cout << "Them doc gia thanh cong.\n";
            else std::cout << "Them doc gia that bai. Kiem tra thong tin hoac ma doc gia bi trung.\n";
            break;
        }
        case 5: library.displayAllUsers(); break;
        case 6: {
            int userId = readInt("Ma doc gia: ");
            int bookId = readInt("Ma sach can muon: ");
            if (library.borrowBook(userId, bookId)) std::cout << "Muon sach thanh cong.\n";
            else std::cout << "Muon sach that bai. Kiem tra ma, so luong sach va gioi han 5 cuon/doc gia.\n";
            break;
        }
        case 7: {
            int userId = readInt("Ma doc gia: ");
            int bookId = readInt("Ma sach can tra: ");
            if (library.returnBook(userId, bookId)) std::cout << "Tra sach thanh cong.\n";
            else std::cout << "Tra sach that bai. Doc gia co the chua muon cuon sach nay.\n";
            break;
        }
        default: std::cout << "Chuc nang khong hop le. Vui long chon lai.\n";
        }
    }
    return 0;
}
