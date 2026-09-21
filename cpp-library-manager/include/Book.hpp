#pragma once

#include <string>

class Book {
public:
    Book(int id, std::string title, std::string author, int year, std::string isbn);

    int getId() const;
    const std::string& getTitle() const;
    const std::string& getAuthor() const;
    int getYear() const;
    const std::string& getIsbn() const;
    bool isAvailable() const;
    void setAvailable(bool available);

    // Pipe-delimited line for persistence: id|title|author|year|isbn|available
    std::string serialize() const;
    static Book deserialize(const std::string& line);

private:
    int id_;
    std::string title_;
    std::string author_;
    int year_;
    std::string isbn_;
    bool available_;
};
