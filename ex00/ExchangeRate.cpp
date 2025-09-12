/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExchangeRate.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyoshid <akyoshid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:54:44 by akyoshid          #+#    #+#             */
/*   Updated: 2025/09/12 12:49:21 by akyoshid         ###   ########.fr       */
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
#include "ExchangeRate.hpp"
#include "utils.hpp"

const size_t ExchangeRate::dateLen_ = 10;

ExchangeRate::ExchangeRate(const std::string& filename)
    : filename_(filename), lineCount_(0) {
    std::ifstream ifs(filename_.c_str());
    if (!ifs.is_open())
        throw std::runtime_error(createErrorMessage(0, 0, "could not open"));
    std::string line = "";
    std::getline(ifs, line);
    ++lineCount_;
    if (line != getHeader())
        throw std::runtime_error(
                createErrorMessage(lineCount_, 0, "invalid header"));
    while (std::getline(ifs, line)) {
        ++lineCount_;
        checkFormat(line);
        database_.insert(
            std::make_pair(
                extractDate(line), extractValue(line.substr(getValueIndex()))));
    }
    if (ifs.bad())
        throw std::runtime_error(createErrorMessage(0, 0, "could not read"));
}

ExchangeRate::~ExchangeRate() {
}

std::string ExchangeRate::getHeader() const {
    return "date,exchange_rate";
}

std::string ExchangeRate::getSeparator() const {
    return ",";
}

size_t ExchangeRate::getSeparatorLen() const {
    return getSeparator().length();
}

size_t ExchangeRate::getValueIndex() const {
    return dateLen_ + getSeparatorLen();
}

double ExchangeRate::getValueMax() const {
    return std::numeric_limits<double>::infinity();
}

void ExchangeRate::checkFormat(const std::string& line) const {
    checkLineLen(line);
    checkDateFormat(line);
    checkSeparatorFormat(line.substr(dateLen_, getSeparatorLen()));
    checkValueFormat(line.substr(getValueIndex()));
}

void ExchangeRate::checkLineLen(const std::string& line) const {
    if (line.length() < getValueIndex() + 1) {
        throw std::runtime_error(createErrorMessage(
                    lineCount_, line.length(), "invalid line length"));
    }
}

void ExchangeRate::checkDateFormat(const std::string& line) const {
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

void ExchangeRate::checkSeparatorFormat(const std::string& separator) const {
    std::string format = getSeparator();
    for (size_t i = 0; i < getSeparatorLen(); ++i) {
        if (format[i] != separator[i]) {
            throw std::runtime_error(createErrorMessage(
                        lineCount_, dateLen_ + 1 + i, "invalid separator format"));
        }
    }
}

void ExchangeRate::checkValueFormat(const std::string& value) const {
    bool hasDot = false;
    bool hasDigit = false;
    for (size_t i = 0; i < value.length(); ++i) {
        if (i == 0 && (value[i] == '+' || value[i] == '-')) {
            continue;
        } else if (value[i] == '.') {
            if (hasDot == true) {
                throw std::runtime_error(createErrorMessage(lineCount_,
                            getValueIndex() + i + 1, "invalid value"));
            }
            hasDot = true;
        } else if (std::isdigit(static_cast<unsigned char>(value[i]))) {
            hasDigit = true;
        } else {
            throw std::runtime_error(createErrorMessage(
                        lineCount_, getValueIndex() + i + 1, "invalid value"));
        }
    }
    if (hasDigit == false) {
        throw std::runtime_error(createErrorMessage(
                    lineCount_, getValueIndex() + 1, "invalid value"));
    }
}

int ExchangeRate::extractDate(const std::string& line) const {
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

double ExchangeRate::extractValue(const std::string& value) const {
    char* end;
    errno = 0;
    double ret = std::strtod(value.c_str(), &end);
    if (*end != '\0') {
        throw std::runtime_error(createErrorMessage(
                    lineCount_, (end - value.c_str()) + getValueIndex() + 1,
                    "invalid value"));
    }
    if (ret < 0) {
        throw std::runtime_error(createErrorMessage(
                    lineCount_, getValueIndex() + 1,
                    "negative value"));
    } else if (ret > getValueMax()) {
        throw std::runtime_error(createErrorMessage(
                    lineCount_, getValueIndex() + 1,
                    "exceeding maximum value"));
    }
    if (errno == ERANGE) {
        throw std::runtime_error(createErrorMessage(
                    lineCount_, getValueIndex() + 1,
                    "value out of double range"));
    }
    return ret;
}

std::string ExchangeRate::createErrorMessage(
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
