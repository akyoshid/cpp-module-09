/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADatabase.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyoshid <akyoshid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 19:41:25 by akyoshid          #+#    #+#             */
/*   Updated: 2025/09/12 15:48:20 by akyoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cerrno>
#include <limits>
#include "ADatabase.hpp"
#include "utils.hpp"

const size_t ADatabase::dateLen_ = 10;

ADatabase::ADatabase(
    const std::string& filename, const std::string& header,
    const std::string& separator, double valueMax)
    : filename_(filename), header_(header), separator_(separator),
        valueIndex_(dateLen_ + separator_.length()), valueMax_(valueMax),
        lineCount_(0) {
    ifs_.open(filename_.c_str());
    if (!ifs_.is_open())
        throw std::runtime_error(createErrorMessage(0, 0, "could not open"));
    std::string line = "";
    std::getline(ifs_, line);
    ++lineCount_;
    if (line != header_)
        throw std::runtime_error(
                createErrorMessage(lineCount_, 0, "invalid header"));
    if (ifs_.bad())
        throw std::runtime_error(createErrorMessage(0, 0, "could not read"));
}

ADatabase::~ADatabase() {
}

void ADatabase::checkFormat(const std::string& line) const {
    checkLineLen(line);
    checkDateFormat(line);
    checkSeparatorFormat(line.substr(dateLen_, separator_.length()));
    checkValueFormat(line.substr(valueIndex_));
}

void ADatabase::checkLineLen(const std::string& line) const {
    if (line.length() < valueIndex_ + 1) {
        throw std::runtime_error(createErrorMessage(
                    lineCount_, line.length(), "invalid format"));
    }
}

void ADatabase::checkDateFormat(const std::string& line) const {
    for (size_t i = 0; i < dateLen_; ++i) {
        if (i == 4 || i == 7) {
            if (line[i] != '-') {
                throw std::runtime_error(createErrorMessage(
                            lineCount_, i + 1, "invalid date format"));
            }
        } else {
            if (std::isdigit(static_cast<unsigned char>(line[i])) == false) {
                throw std::runtime_error(createErrorMessage(
                            lineCount_, i + 1, "invalid date format"));
            }
        }
    }
}

void ADatabase::checkSeparatorFormat(const std::string& separator) const {
    for (size_t i = 0; i < separator.length(); ++i) {
        if (separator_[i] != separator[i]) {
            throw std::runtime_error(createErrorMessage(
                        lineCount_, dateLen_ + 1 + i, "invalid separator format"));
        }
    }
}

void ADatabase::checkValueFormat(const std::string& value) const {
    bool hasDot = false;
    bool hasDigit = false;
    for (size_t i = 0; i < value.length(); ++i) {
        if (i == 0 && (value[i] == '+' || value[i] == '-')) {
            continue;
        } else if (value[i] == '.') {
            if (hasDot == true) {
                throw std::runtime_error(createErrorMessage(lineCount_,
                            valueIndex_ + i + 1, "invalid value"));
            }
            hasDot = true;
        } else if (std::isdigit(static_cast<unsigned char>(value[i]))) {
            hasDigit = true;
        } else {
            throw std::runtime_error(createErrorMessage(
                        lineCount_, valueIndex_ + i + 1, "invalid value"));
        }
    }
    if (hasDigit == false) {
        throw std::runtime_error(createErrorMessage(
                    lineCount_, valueIndex_ + 1, "invalid value"));
    }
}

int ADatabase::extractDate(const std::string& line) const {
    int year = 0;
    for (int i = 0; i < 4; ++i) {
        year *= 10;
        year += line[i] - '0';
    }
    int month = 0;
    for (int i = 5; i < 7; ++i) {
        month *= 10;
        month += line[i] - '0';
    }
    if (month == 0 || month > 12) {
        throw std::runtime_error(createErrorMessage(
                    lineCount_, 6, "invalid month"));
    }
    int date = 0;
    for (int i = 8; i < 10; ++i) {
        date *= 10;
        date += line[i] - '0';
    }
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
            if (date == 0 || date > 29) {
                throw std::runtime_error(createErrorMessage(
                            lineCount_, 9, "invalid date"));
            }
        } else {
            if (date == 0 || date > 28) {
                throw std::runtime_error(createErrorMessage(
                            lineCount_, 9, "invalid date"));
            }
        }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (date == 0 || date > 30) {
            throw std::runtime_error(createErrorMessage(
                        lineCount_, 9, "invalid date"));
        }
    } else {
        if (date == 0 || date > 31) {
            throw std::runtime_error(createErrorMessage(
                        lineCount_, 9, "invalid date"));
        }
    }
    return year * 10000 + month * 100 + date;
}

double ADatabase::extractValue(const std::string& value) const {
    char* end;
    errno = 0;
    double ret = std::strtod(value.c_str(), &end);
    if (*end != '\0') {
        throw std::runtime_error(createErrorMessage(
                    lineCount_, (end - value.c_str()) + valueIndex_ + 1,
                    "invalid value"));
    }
    if (ret < 0) {
        throw std::runtime_error(createErrorMessage(
                    lineCount_, valueIndex_ + 1,
                    "negative value"));
    } else if (ret > valueMax_) {
        throw std::runtime_error(createErrorMessage(
                    lineCount_, valueIndex_ + 1,
                    "exceeding maximum value"));
    }
    if (errno == ERANGE) {
        throw std::runtime_error(createErrorMessage(
                    lineCount_, valueIndex_ + 1,
                    "value out of double range"));
    }
    return ret;
}

std::string ADatabase::createErrorMessage(
    size_t line, size_t column, const std::string& msg) const {
    if (column <= 0) {
        if (line <= 0) {
            return "Error: " + filename_ + ": " + msg;
        } else {
            return "Error: " + filename_ + ":" + itostr(line) + ": " + msg;
        }
    } else {
        return "Error: " + filename_ + ":" + itostr(line) + ":" + itostr(column) + ": " + msg;
    }
}

double ADatabase::searchLE(int key) const {
    if (database_.empty()) {
        throw std::runtime_error("Error: searchLE: database is empty");
    }
    std::map<int, double>::const_iterator it = database_.upper_bound(key);
    if (it == database_.begin()) {
        throw std::runtime_error(
            "Error: searchLE: no date found on or before "
                + createDateStr(key));
    }
    return (--it)->second;
}

std::string ADatabase::createDateStr(int date) {
    int year = date / 10000;
    int month = (date / 100) % 100;
    int day = date % 100;;
    std::string yearStr = "";
    if (year < 10) {
        yearStr += "000";
    } else if (year < 100) {
        yearStr += "00";
    } else if (year < 1000) {
        yearStr += "0";
    }
    yearStr += itostr(year);
    std::string monthStr = "";
    if (month < 10) {
        monthStr += "0";
    }
    monthStr += itostr(month);
    std::string dayStr = "";
    if (day < 10) {
        dayStr += "0";
    }
    dayStr += itostr(day);
    return yearStr + "-" + monthStr + "-" + dayStr;
}
