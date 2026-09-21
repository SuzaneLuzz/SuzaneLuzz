#include "Date.hpp"

#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {}

Date Date::today() {
    std::time_t now = std::time(nullptr);
    std::tm local = *std::localtime(&now);
    return Date(local.tm_mday, local.tm_mon + 1, local.tm_year + 1900);
}

Date Date::addDays(int days) const {
    std::tm timeStruct{};
    timeStruct.tm_mday = day + days;
    timeStruct.tm_mon = month - 1;
    timeStruct.tm_year = year - 1900;
    timeStruct.tm_hour = 12; // Midday avoids DST rollovers shifting the date by one.

    std::time_t asTime = std::mktime(&timeStruct);
    if (asTime == -1) {
        throw std::runtime_error("Nao foi possivel calcular a nova data");
    }

    std::tm normalized = *std::localtime(&asTime);
    return Date(normalized.tm_mday, normalized.tm_mon + 1, normalized.tm_year + 1900);
}

bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

bool Date::operator==(const Date& other) const {
    return day == other.day && month == other.month && year == other.year;
}

bool Date::operator<=(const Date& other) const {
    return (*this < other) || (*this == other);
}

std::string Date::toString() const {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << day << '/'
        << std::setw(2) << std::setfill('0') << month << '/'
        << year;
    return oss.str();
}

Date Date::fromString(const std::string& text) {
    int d = 0;
    int m = 0;
    int y = 0;
    char sep1 = '\0';
    char sep2 = '\0';

    std::istringstream iss(text);
    if (!(iss >> d >> sep1 >> m >> sep2 >> y) || sep1 != '/' || sep2 != '/') {
        throw std::invalid_argument("Formato de data invalido, use DD/MM/AAAA: " + text);
    }
    return Date(d, m, y);
}
