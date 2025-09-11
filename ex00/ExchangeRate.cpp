/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExchangeRate.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyoshid <akyoshid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:54:44 by akyoshid          #+#    #+#             */
/*   Updated: 2025/09/11 19:29:49 by akyoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cerrno>
#include "ExchangeRate.hpp"
#include "utils.hpp"

ExchangeRate::ExchangeRate(const std::string& filename)
    : filename_(filename), lineCount_(0) {
    std::ifstream ifs(filename_.c_str());
    if (!ifs.is_open())
        throw std::runtime_error(createErrorMessage(0, 0, "could not open"));
    std::string line = "";
    std::getline(ifs, line);
    ++lineCount_;
    if (line != "date,exchange_rate")
        throw std::runtime_error(
                createErrorMessage(lineCount_, 0, "invalid header"));
    while (std::getline(ifs, line)) {
        ++lineCount_;
        checkFormat(line);
        database_.insert(
            std::make_pair(extractDate(line), extractRate(line)));
    }
    if (ifs.bad())
        throw std::runtime_error(createErrorMessage(0, 0, "could not read"));
}

ExchangeRate::~ExchangeRate() {
}

void ExchangeRate::checkFormat(const std::string& line) const {
    if (line.length() < 12) {
        throw std::runtime_error(createErrorMessage(
                    lineCount_, line.length(), "invalid line length"));
    }
    bool hasDot = false;
    bool hasDigit = false;
    for (size_t i = 0; i < line.length(); ++i) {
        if (i < 10) {
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
        } else if (i == 10) {
            if (line[i] != ',') {
                throw std::runtime_error(createErrorMessage(
                            lineCount_, i + 1, "invalid format"));
            }
        } else {
            if (line[i] == '.') {
                if (hasDot == true) {
                    throw std::runtime_error(createErrorMessage(
                                lineCount_, i + 1, "invalid price"));
                }
                hasDot = true;
            } else if (std::isdigit(static_cast<unsigned char>(line[i]))) {
                hasDigit = true;
            } else {
                throw std::runtime_error(createErrorMessage(
                            lineCount_, i + 1, "invalid price"));
            }
        }
    }
    if (hasDigit == false) {
        throw std::runtime_error(createErrorMessage(
                    lineCount_, 12, "invalid price"));
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

double ExchangeRate::extractRate(const std::string& line) const {
    char* end;
    errno = 0;
    double ret = std::strtod(line.c_str() + 11, &end);
    if (*end != '\0') {
        throw std::runtime_error(createErrorMessage(
                    lineCount_, end - line.c_str() + 1, "invalid price"));
    }
    if (errno == ERANGE) {
        throw std::runtime_error(createErrorMessage(
                    lineCount_, 12, "price out of double range"));
    }
    return ret;
}

std::string ExchangeRate::createErrorMessage(
    int line, int column, const std::string& msg) const {
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
