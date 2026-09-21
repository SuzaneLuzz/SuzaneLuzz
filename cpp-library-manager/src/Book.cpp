#include "Book.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>

Book::Book(int id, std::string title, std::string author, int year, std::string isbn)
    : id_(id),
      title_(std::move(title)),
      author_(std::move(author)),
      year_(year),
      isbn_(std::move(isbn)),
      available_(true) {}

int Book::getId() const { return id_; }
const std::string& Book::getTitle() const { return title_; }
const std::string& Book::getAuthor() const { return author_; }
int Book::getYear() const { return year_; }
const std::string& Book::getIsbn() const { return isbn_; }
bool Book::isAvailable() const { return available_; }
void Book::setAvailable(bool available) { available_ = available; }

std::string Book::serialize() const {
    std::ostringstream oss;
    oss << id_ << '|' << title_ << '|' << author_ << '|' << year_ << '|' << isbn_ << '|'
        << (available_ ? 1 : 0);
    return oss.str();
}

namespace {
std::string nextField(std::istringstream& iss) {
    std::string field;
    std::getline(iss, field, '|');
    return field;
}
} // namespace

Book Book::deserialize(const std::string& line) {
    std::istringstream iss(line);
    std::string idField = nextField(iss);
    std::string title = nextField(iss);
    std::string author = nextField(iss);
    std::string yearField = nextField(iss);
    std::string isbn = nextField(iss);
    std::string availableField = nextField(iss);

    if (idField.empty() || yearField.empty()) {
        throw std::runtime_error("Linha de livro invalida no arquivo de dados: " + line);
    }

    Book book(std::stoi(idField), title, author, std::stoi(yearField), isbn);
    book.setAvailable(availableField == "1");
    return book;
}
