#include "Member.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>

Member::Member(int id, std::string name, std::string email)
    : id_(id), name_(std::move(name)), email_(std::move(email)) {}

int Member::getId() const { return id_; }
const std::string& Member::getName() const { return name_; }
const std::string& Member::getEmail() const { return email_; }

std::string Member::serialize() const {
    std::ostringstream oss;
    oss << id_ << '|' << name_ << '|' << email_;
    return oss.str();
}

namespace {
std::string nextField(std::istringstream& iss) {
    std::string field;
    std::getline(iss, field, '|');
    return field;
}
} // namespace

Member Member::deserialize(const std::string& line) {
    std::istringstream iss(line);
    std::string idField = nextField(iss);
    std::string name = nextField(iss);
    std::string email = nextField(iss);

    if (idField.empty()) {
        throw std::runtime_error("Linha de membro invalida no arquivo de dados: " + line);
    }

    return Member(std::stoi(idField), name, email);
}
