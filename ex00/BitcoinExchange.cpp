/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyoshid <akyoshid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 05:41:18 by akyoshid          #+#    #+#             */
/*   Updated: 2025/09/12 16:21:50 by akyoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <stdexcept>
#include "BitcoinExchange.hpp"
#include "ExchangeRate.hpp"

BitcoinExchange::BitcoinExchange(
    const std::string& filename, const ExchangeRate& rate)
    : ADatabase(filename, "date | value", " | ", 1000) {
    std::string line = "";
    while (std::getline(ifs_, line)) {
        ++lineCount_;
        try {
            checkFormat(line);
            exchange(extractDate(line), extractValue(line.substr(valueIndex_)),
                rate);
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    if (ifs_.bad())
        throw std::runtime_error(createErrorMessage(0, 0, "could not read"));
}

BitcoinExchange::~BitcoinExchange() {
}

void BitcoinExchange::exchange(
    int date, double value, const ExchangeRate& rate) {
    try {
        double searchedValue = rate.searchLE(date);
        std::cout << createDateStr(date) << " => "
            << value << " = " << value * searchedValue << std::endl;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
