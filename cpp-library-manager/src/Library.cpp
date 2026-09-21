#include "Library.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>

#include "Exceptions.hpp"

namespace fs = std::filesystem;

Library::Library(std::string dataDirectory)
    : dataDirectory_(std::move(dataDirectory)), nextBookId_(1), nextMemberId_(1), nextLoanId_(1) {}

std::string Library::booksFilePath() const { return dataDirectory_ + "/books.txt"; }
std::string Library::membersFilePath() const { return dataDirectory_ + "/members.txt"; }
std::string Library::loansFilePath() const { return dataDirectory_ + "/loans.txt"; }

void Library::load() {
    books_.clear();
    members_.clear();
    loans_.clear();

    if (fs::exists(booksFilePath())) {
        std::ifstream in(booksFilePath());
        std::string line;
        while (std::getline(in, line)) {
            if (line.empty()) continue;
            books_.push_back(Book::deserialize(line));
        }
    }

    if (fs::exists(membersFilePath())) {
        std::ifstream in(membersFilePath());
        std::string line;
        while (std::getline(in, line)) {
            if (line.empty()) continue;
            members_.push_back(Member::deserialize(line));
        }
    }

    if (fs::exists(loansFilePath())) {
        std::ifstream in(loansFilePath());
        std::string line;
        while (std::getline(in, line)) {
            if (line.empty()) continue;
            loans_.push_back(Loan::deserialize(line));
        }
    }

    for (const auto& book : books_) {
        nextBookId_ = std::max(nextBookId_, book.getId() + 1);
    }
    for (const auto& member : members_) {
        nextMemberId_ = std::max(nextMemberId_, member.getId() + 1);
    }
    for (const auto& loan : loans_) {
        nextLoanId_ = std::max(nextLoanId_, loan.getId() + 1);
    }
}

void Library::save() const {
    fs::create_directories(dataDirectory_);

    std::ofstream booksOut(booksFilePath(), std::ios::trunc);
    for (const auto& book : books_) {
        booksOut << book.serialize() << '\n';
    }

    std::ofstream membersOut(membersFilePath(), std::ios::trunc);
    for (const auto& member : members_) {
        membersOut << member.serialize() << '\n';
    }

    std::ofstream loansOut(loansFilePath(), std::ios::trunc);
    for (const auto& loan : loans_) {
        loansOut << loan.serialize() << '\n';
    }
}

Book& Library::addBook(const std::string& title, const std::string& author, int year,
                        const std::string& isbn) {
    books_.emplace_back(nextBookId_++, title, author, year, isbn);
    return books_.back();
}

void Library::removeBook(int bookId) {
    auto it = std::find_if(books_.begin(), books_.end(),
                            [bookId](const Book& b) { return b.getId() == bookId; });
    if (it == books_.end()) {
        throw BookNotFoundException("Livro nao encontrado: id " + std::to_string(bookId));
    }
    books_.erase(it);
}

Book& Library::findBookById(int bookId) {
    auto it = std::find_if(books_.begin(), books_.end(),
                            [bookId](const Book& b) { return b.getId() == bookId; });
    if (it == books_.end()) {
        throw BookNotFoundException("Livro nao encontrado: id " + std::to_string(bookId));
    }
    return *it;
}

namespace {
std::string toLower(const std::string& text) {
    std::string result = text;
    std::transform(result.begin(), result.end(), result.begin(),
                    [](unsigned char c) { return std::tolower(c); });
    return result;
}
} // namespace

std::vector<Book> Library::searchByTitle(const std::string& query) const {
    std::vector<Book> results;
    std::string needle = toLower(query);
    std::copy_if(books_.begin(), books_.end(), std::back_inserter(results),
                 [&needle](const Book& b) { return toLower(b.getTitle()).find(needle) != std::string::npos; });
    return results;
}

std::vector<Book> Library::searchByAuthor(const std::string& query) const {
    std::vector<Book> results;
    std::string needle = toLower(query);
    std::copy_if(books_.begin(), books_.end(), std::back_inserter(results),
                 [&needle](const Book& b) { return toLower(b.getAuthor()).find(needle) != std::string::npos; });
    return results;
}

std::vector<Book> Library::listBooksSortedByTitle() const {
    std::vector<Book> sorted = books_;
    std::sort(sorted.begin(), sorted.end(),
              [](const Book& a, const Book& b) { return toLower(a.getTitle()) < toLower(b.getTitle()); });
    return sorted;
}

std::vector<Book> Library::listBooksSortedByYear() const {
    std::vector<Book> sorted = books_;
    std::sort(sorted.begin(), sorted.end(),
              [](const Book& a, const Book& b) { return a.getYear() < b.getYear(); });
    return sorted;
}

const std::vector<Book>& Library::getBooks() const { return books_; }

Member& Library::addMember(const std::string& name, const std::string& email) {
    members_.emplace_back(nextMemberId_++, name, email);
    return members_.back();
}

void Library::removeMember(int memberId) {
    auto it = std::find_if(members_.begin(), members_.end(),
                            [memberId](const Member& m) { return m.getId() == memberId; });
    if (it == members_.end()) {
        throw MemberNotFoundException("Membro nao encontrado: id " + std::to_string(memberId));
    }
    members_.erase(it);
}

Member& Library::findMemberById(int memberId) {
    auto it = std::find_if(members_.begin(), members_.end(),
                            [memberId](const Member& m) { return m.getId() == memberId; });
    if (it == members_.end()) {
        throw MemberNotFoundException("Membro nao encontrado: id " + std::to_string(memberId));
    }
    return *it;
}

const std::vector<Member>& Library::getMembers() const { return members_; }

Loan& Library::borrowBook(int memberId, int bookId, int loanDays) {
    findMemberById(memberId); // Throws MemberNotFoundException if missing.
    Book& book = findBookById(bookId);

    if (!book.isAvailable()) {
        throw BookNotAvailableException("Livro indisponivel para emprestimo: id " +
                                         std::to_string(bookId));
    }

    Date loanDate = Date::today();
    Date dueDate = loanDate.addDays(loanDays);

    book.setAvailable(false);
    loans_.emplace_back(nextLoanId_++, bookId, memberId, loanDate, dueDate);
    return loans_.back();
}

void Library::returnBook(int loanId) {
    auto it = std::find_if(loans_.begin(), loans_.end(),
                            [loanId](const Loan& l) { return l.getId() == loanId; });
    if (it == loans_.end()) {
        throw LoanNotFoundException("Emprestimo nao encontrado: id " + std::to_string(loanId));
    }
    if (it->isReturned()) {
        throw LibraryException("Emprestimo ja foi devolvido: id " + std::to_string(loanId));
    }

    it->markReturned(Date::today());
    findBookById(it->getBookId()).setAvailable(true);
}

std::vector<Loan> Library::listActiveLoans() const {
    std::vector<Loan> results;
    std::copy_if(loans_.begin(), loans_.end(), std::back_inserter(results),
                 [](const Loan& l) { return !l.isReturned(); });
    return results;
}

std::vector<Loan> Library::listOverdueLoans() const {
    std::vector<Loan> results;
    Date today = Date::today();
    std::copy_if(loans_.begin(), loans_.end(), std::back_inserter(results),
                 [&today](const Loan& l) { return !l.isReturned() && l.isOverdue(today); });
    return results;
}

const std::vector<Loan>& Library::getLoans() const { return loans_; }
