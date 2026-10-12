#include "library.h"
#include <algorithm>
#include <charconv>
#include <filesystem>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

namespace {
std::optional<int> readInt(std::string_view prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) return std::nullopt;
        int value{};
        const char* begin = line.data();
        const char* end = begin + line.size();
        const auto [position, error] = std::from_chars(begin, end, value);
        if (error == std::errc{} && position == end) return value;
        std::cout << "Vui long nhap mot so nguyen hop le.\n";
    }
}
std::optional<std::string> readText(std::string_view prompt) {
    std::cout << prompt;
    std::string value;
    if (!std::getline(std::cin, value)) return std::nullopt;
    return value;
}
void showMenu() {
    std::cout << "\n========== QUAN LY THU VIEN (C++20) ==========\n"
              << "1. Them sach\n2. Hien thi sach\n3. Tim sach theo ten\n"
              << "4. Them doc gia\n5. Hien thi doc gia\n6. Muon sach\n"
              << "7. Tra sach\n0. Luu va thoat\n===============================================\n";
}
void printBook(const Book& book) {
    std::cout << "Ma sach: " << book.id() << " | Ten: " << book.title()
              << " | Tac gia: " << book.author() << " | Nam: " << book.year()
              << " | Tong: " << book.totalCopies() << " | Con: " << book.availableCopies()
              << " | Dang muon: " << book.borrowedCopies() << '\n';
}
void printUser(const User& user) {
    std::cout << "Ma doc gia: " << user.id() << " | Ho ten: " << user.name()
              << " | Dien thoai: " << user.phoneNumber() << " | Email: " << user.email()
              << " | Dang muon: " << user.borrowedBookIds().size() << '/' << User::maxBorrowedBooks << '\n';
    std::vector<int> ids(user.borrowedBookIds().begin(), user.borrowedBookIds().end());
    std::sort(ids.begin(), ids.end());
    std::cout << "  Ma sach dang muon: ";
    if (ids.empty()) std::cout << "khong co";
    for (std::size_t i = 0; i < ids.size(); ++i) {
        if (i != 0) std::cout << ", ";
        std::cout << ids[i];
    }
    std::cout << '\n';
}
bool addBookFromInput(Library& library) {
    const auto id = readInt("Ma sach: "); if (!id) return false;
    const auto title = readText("Ten sach: "); if (!title) return false;
    const auto author = readText("Tac gia: "); if (!author) return false;
    const auto year = readInt("Nam xuat ban: "); if (!year) return false;
    const auto copies = readInt("Tong so ban: "); if (!copies) return false;
    try {
        if (!library.addBook(Book(*id, *title, *author, *year, *copies))) {
            std::cout << "Ma sach da ton tai.\n"; return false;
        }
        std::cout << "Them sach thanh cong.\n"; return true;
    } catch (const std::invalid_argument& error) {
        std::cout << "Du lieu khong hop le: " << error.what() << '\n'; return false;
    }
}
bool addUserFromInput(Library& library) {
    const auto id = readInt("Ma doc gia: "); if (!id) return false;
    const auto name = readText("Ho ten: "); if (!name) return false;
    const auto phone = readText("So dien thoai (co the de trong): "); if (!phone) return false;
    const auto email = readText("Email (co the de trong): "); if (!email) return false;
    try {
        if (!library.addUser(User(*id, *name, *phone, *email))) {
            std::cout << "Ma doc gia da ton tai.\n"; return false;
        }
        std::cout << "Them doc gia thanh cong.\n"; return true;
    } catch (const std::invalid_argument& error) {
        std::cout << "Du lieu khong hop le: " << error.what() << '\n'; return false;
    }
}
bool borrowFromInput(Library& library) {
    const auto userId = readInt("Ma doc gia: "); if (!userId) return false;
    const auto bookId = readInt("Ma sach: "); if (!bookId) return false;
    if (!library.borrowBook(*userId, *bookId)) {
        std::cout << "Muon sach that bai. Kiem tra ma, so luong sach va gioi han 5 cuon.\n";
        return false;
    }
    std::cout << "Muon sach thanh cong.\n"; return true;
}
bool returnFromInput(Library& library) {
    const auto userId = readInt("Ma doc gia: "); if (!userId) return false;
    const auto bookId = readInt("Ma sach: "); if (!bookId) return false;
    if (!library.returnBook(*userId, *bookId)) {
        std::cout << "Tra sach that bai. Doc gia co the khong dang muon sach nay.\n";
        return false;
    }
    std::cout << "Tra sach thanh cong.\n"; return true;
}
void seedSampleData(Library& library) {
    static_cast<void>(library.addBook(Book(1, "Lap trinh C++", "Bjarne Stroustrup", 2013, 3)));
    static_cast<void>(library.addBook(Book(2, "Clean Code", "Robert C. Martin", 2008, 2)));
    static_cast<void>(library.addUser(User(1, "Nguyen An", "0900000001", "an@example.com")));
}
}

int main() {
    const std::filesystem::path dataPath{"library-data.txt"};
    try {
        Library library;
        if (std::filesystem::exists(dataPath)) {
            library = Library::loadFromFile(dataPath);
            std::cout << "Da nap du lieu tu " << dataPath.string() << ".\n";
        } else {
            seedSampleData(library);
            std::cout << "Tao du lieu mau. Du lieu duoc luu vao " << dataPath.string() << ".\n";
        }
        while (true) {
            showMenu();
            const auto choice = readInt("Chon chuc nang: ");
            if (!choice || *choice == 0) break;
            bool changed = false;
            switch (*choice) {
            case 1: changed = addBookFromInput(library); break;
            case 2: {
                const auto books = library.allBooks();
                if (books.empty()) std::cout << "Thu vien chua co sach.\n";
                for (const auto reference : books) printBook(reference.get());
                break;
            }
            case 3: {
                const auto keyword = readText("Ten sach can tim: ");
                if (!keyword) break;
                const auto matches = library.searchBooks(*keyword);
                if (matches.empty()) std::cout << "Khong tim thay sach phu hop.\n";
                for (const auto reference : matches) printBook(reference.get());
                break;
            }
            case 4: changed = addUserFromInput(library); break;
            case 5: {
                const auto users = library.allUsers();
                if (users.empty()) std::cout << "Chua co doc gia nao.\n";
                for (const auto reference : users) printUser(reference.get());
                break;
            }
            case 6: changed = borrowFromInput(library); break;
            case 7: changed = returnFromInput(library); break;
            default: std::cout << "Chuc nang khong hop le.\n"; break;
            }
            if (!std::cin) break;
            if (changed) {
                library.saveToFile(dataPath);
                std::cout << "Da luu du lieu.\n";
            }
        }
        library.saveToFile(dataPath);
        std::cout << "Da luu du lieu va thoat chuong trinh.\n";
    } catch (const std::exception& error) {
        std::cerr << "Loi: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
