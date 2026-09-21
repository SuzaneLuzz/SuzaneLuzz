// Minimal assert-based test suite: no external test framework required.
#include <cassert>
#include <filesystem>
#include <iostream>

#include "Book.hpp"
#include "Date.hpp"
#include "Exceptions.hpp"
#include "Library.hpp"

namespace {

int testsRun = 0;

#define RUN_TEST(fn)                    \
    do {                                \
        std::cout << "  - " #fn "\n";   \
        fn();                           \
        ++testsRun;                     \
    } while (0)

void testDateAddDaysCrossesMonth() {
    Date start(28, 2, 2023);
    Date result = start.addDays(5);
    assert(result.day == 5);
    assert(result.month == 3);
    assert(result.year == 2023);
}

void testDateOrdering() {
    Date earlier(1, 1, 2024);
    Date later(2, 1, 2024);
    assert(earlier < later);
    assert(!(later < earlier));
    assert(earlier <= earlier);
}

void testBookSerializationRoundTrip() {
    Book book(7, "O Senhor dos Aneis", "J.R.R. Tolkien", 1954, "978-0261102385");
    book.setAvailable(false);

    Book restored = Book::deserialize(book.serialize());

    assert(restored.getId() == 7);
    assert(restored.getTitle() == "O Senhor dos Aneis");
    assert(restored.getAuthor() == "J.R.R. Tolkien");
    assert(restored.getYear() == 1954);
    assert(restored.getIsbn() == "978-0261102385");
    assert(restored.isAvailable() == false);
}

void testAddAndFindBook() {
    Library library("test_data_add_find");
    Book& book = library.addBook("Dom Casmurro", "Machado de Assis", 1899, "111");
    Book& found = library.findBookById(book.getId());
    assert(found.getTitle() == "Dom Casmurro");
}

void testRemovingMissingBookThrows() {
    Library library("test_data_missing_book");
    bool threw = false;
    try {
        library.removeBook(999);
    } catch (const BookNotFoundException&) {
        threw = true;
    }
    assert(threw);
}

void testBorrowAndReturnFlow() {
    Library library("test_data_borrow_return");
    Member& member = library.addMember("Ana Silva", "ana@example.com");
    Book& book = library.addBook("Clean Code", "Robert C. Martin", 2008, "222");

    Loan& loan = library.borrowBook(member.getId(), book.getId());
    assert(loan.isReturned() == false);
    assert(library.findBookById(book.getId()).isAvailable() == false);

    library.returnBook(loan.getId());
    assert(library.getLoans().front().isReturned() == true);
    assert(library.findBookById(book.getId()).isAvailable() == true);
}

void testBorrowingUnavailableBookThrows() {
    Library library("test_data_unavailable");
    Member& member = library.addMember("Bruno Costa", "bruno@example.com");
    Book& book = library.addBook("1984", "George Orwell", 1949, "333");

    library.borrowBook(member.getId(), book.getId());

    bool threw = false;
    try {
        library.borrowBook(member.getId(), book.getId());
    } catch (const BookNotAvailableException&) {
        threw = true;
    }
    assert(threw);
}

void testSaveAndLoadRoundTrip() {
    const std::string dir = "test_data_persistence";
    std::filesystem::remove_all(dir);

    {
        Library library(dir);
        library.addBook("Neuromancer", "William Gibson", 1984, "444");
        library.addMember("Carla Dias", "carla@example.com");
        library.save();
    }

    Library reloaded(dir);
    reloaded.load();
    assert(reloaded.getBooks().size() == 1);
    assert(reloaded.getMembers().size() == 1);
    assert(reloaded.getBooks().front().getTitle() == "Neuromancer");

    std::filesystem::remove_all(dir);
}

void testSearchIsCaseInsensitive() {
    Library library("test_data_search");
    library.addBook("The Pragmatic Programmer", "David Thomas", 1999, "555");

    auto results = library.searchByTitle("pragmatic");
    assert(results.size() == 1);
}

void cleanupTestDirectories() {
    for (const auto* dir : {"test_data_add_find", "test_data_missing_book", "test_data_borrow_return",
                             "test_data_unavailable", "test_data_search"}) {
        std::filesystem::remove_all(dir);
    }
}

} // namespace

int main() {
    std::cout << "Executando testes do Library Manager...\n";

    RUN_TEST(testDateAddDaysCrossesMonth);
    RUN_TEST(testDateOrdering);
    RUN_TEST(testBookSerializationRoundTrip);
    RUN_TEST(testAddAndFindBook);
    RUN_TEST(testRemovingMissingBookThrows);
    RUN_TEST(testBorrowAndReturnFlow);
    RUN_TEST(testBorrowingUnavailableBookThrows);
    RUN_TEST(testSaveAndLoadRoundTrip);
    RUN_TEST(testSearchIsCaseInsensitive);

    cleanupTestDirectories();

    std::cout << testsRun << " teste(s) executado(s) com sucesso.\n";
    return 0;
}
