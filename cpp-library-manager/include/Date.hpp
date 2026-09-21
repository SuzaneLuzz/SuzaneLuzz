#pragma once

#include <string>

// Small calendar-date value type used for loan/due dates.
struct Date {
    int day;
    int month;
    int year;

    explicit Date(int d = 1, int m = 1, int y = 1970);

    static Date today();
    Date addDays(int days) const;

    bool operator<(const Date& other) const;
    bool operator==(const Date& other) const;
    bool operator<=(const Date& other) const;

    // Formats as DD/MM/AAAA.
    std::string toString() const;

    // Parses the DD/MM/AAAA format produced by toString().
    static Date fromString(const std::string& text);
};
