#include "Loan.hpp"

#include <sstream>
#include <stdexcept>

Loan::Loan(int id, int bookId, int memberId, Date loanDate, Date dueDate)
    : id_(id),
      bookId_(bookId),
      memberId_(memberId),
      loanDate_(loanDate),
      dueDate_(dueDate),
      returned_(false),
      returnDate_(Date(1, 1, 1970)) {}

int Loan::getId() const { return id_; }
int Loan::getBookId() const { return bookId_; }
int Loan::getMemberId() const { return memberId_; }
const Date& Loan::getLoanDate() const { return loanDate_; }
const Date& Loan::getDueDate() const { return dueDate_; }
bool Loan::isReturned() const { return returned_; }
const Date& Loan::getReturnDate() const { return returnDate_; }

void Loan::markReturned(const Date& returnDate) {
    returned_ = true;
    returnDate_ = returnDate;
}

bool Loan::isOverdue(const Date& referenceDate) const {
    if (returned_) {
        return dueDate_ < returnDate_;
    }
    return dueDate_ < referenceDate;
}

std::string Loan::serialize() const {
    std::ostringstream oss;
    oss << id_ << '|' << bookId_ << '|' << memberId_ << '|' << loanDate_.toString() << '|'
        << dueDate_.toString() << '|' << (returned_ ? 1 : 0) << '|'
        << (returned_ ? returnDate_.toString() : std::string("-"));
    return oss.str();
}

namespace {
std::string nextField(std::istringstream& iss) {
    std::string field;
    std::getline(iss, field, '|');
    return field;
}
} // namespace

Loan Loan::deserialize(const std::string& line) {
    std::istringstream iss(line);
    std::string idField = nextField(iss);
    std::string bookIdField = nextField(iss);
    std::string memberIdField = nextField(iss);
    std::string loanDateField = nextField(iss);
    std::string dueDateField = nextField(iss);
    std::string returnedField = nextField(iss);
    std::string returnDateField = nextField(iss);

    if (idField.empty()) {
        throw std::runtime_error("Linha de emprestimo invalida no arquivo de dados: " + line);
    }

    Loan loan(std::stoi(idField), std::stoi(bookIdField), std::stoi(memberIdField),
              Date::fromString(loanDateField), Date::fromString(dueDateField));

    if (returnedField == "1" && returnDateField != "-") {
        loan.markReturned(Date::fromString(returnDateField));
    }

    return loan;
}
