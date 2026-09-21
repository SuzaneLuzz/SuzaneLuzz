#pragma once

#include <string>

class Member {
public:
    Member(int id, std::string name, std::string email);

    int getId() const;
    const std::string& getName() const;
    const std::string& getEmail() const;

    // Pipe-delimited line for persistence: id|name|email
    std::string serialize() const;
    static Member deserialize(const std::string& line);

private:
    int id_;
    std::string name_;
    std::string email_;
};
