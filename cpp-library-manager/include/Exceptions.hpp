#pragma once

#include <stdexcept>
#include <string>

// Base exception for all library-domain errors.
class LibraryException : public std::runtime_error {
public:
    explicit LibraryException(const std::string& message) : std::runtime_error(message) {}
};

class BookNotFoundException : public LibraryException {
public:
    using LibraryException::LibraryException;
};

class MemberNotFoundException : public LibraryException {
public:
    using LibraryException::LibraryException;
};

class LoanNotFoundException : public LibraryException {
public:
    using LibraryException::LibraryException;
};

class BookNotAvailableException : public LibraryException {
public:
    using LibraryException::LibraryException;
};
