#pragma once

#include <string>

#include "Date.hpp"

class Loan {
public:
    Loan(int id, int bookId, int memberId, Date loanDate, Date dueDate);

    int getId() const;
    int getBookId() const;
    int getMemberId() const;
    const Date& getLoanDate() const;
    const Date& getDueDate() const;
    bool isReturned() const;
    const Date& getReturnDate() const;

    void markReturned(const Date& returnDate);
    bool isOverdue(const Date& referenceDate) const;

    // Pipe-delimited line: id|bookId|memberId|loanDate|dueDate|returned|returnDate
    std::string serialize() const;
    static Loan deserialize(const std::string& line);

private:
    int id_;
    int bookId_;
    int memberId_;
    Date loanDate_;
    Date dueDate_;
    bool returned_;
    Date returnDate_;
};
