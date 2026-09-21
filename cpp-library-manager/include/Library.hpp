#pragma once

#include <string>
#include <vector>

#include "Book.hpp"
#include "Loan.hpp"
#include "Member.hpp"

// Coordinates books, members and loans, including persistence to disk.
class Library {
public:
    explicit Library(std::string dataDirectory = "data");

    void load();
    void save() const;

    Book& addBook(const std::string& title, const std::string& author, int year,
                  const std::string& isbn);
    void removeBook(int bookId);
    Book& findBookById(int bookId);
    std::vector<Book> searchByTitle(const std::string& query) const;
    std::vector<Book> searchByAuthor(const std::string& query) const;
    std::vector<Book> listBooksSortedByTitle() const;
    std::vector<Book> listBooksSortedByYear() const;
    const std::vector<Book>& getBooks() const;

    Member& addMember(const std::string& name, const std::string& email);
    void removeMember(int memberId);
    Member& findMemberById(int memberId);
    const std::vector<Member>& getMembers() const;

    Loan& borrowBook(int memberId, int bookId, int loanDays = 14);
    void returnBook(int loanId);
    std::vector<Loan> listActiveLoans() const;
    std::vector<Loan> listOverdueLoans() const;
    const std::vector<Loan>& getLoans() const;

private:
    std::string dataDirectory_;
    std::vector<Book> books_;
    std::vector<Member> members_;
    std::vector<Loan> loans_;
    int nextBookId_;
    int nextMemberId_;
    int nextLoanId_;

    std::string booksFilePath() const;
    std::string membersFilePath() const;
    std::string loansFilePath() const;
};
