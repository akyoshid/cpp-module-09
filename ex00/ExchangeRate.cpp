/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExchangeRate.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyoshid <akyoshid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:54:44 by akyoshid          #+#    #+#             */
/*   Updated: 2025/09/12 16:51:42 by akyoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <limits>
#include <iostream>
#include "ExchangeRate.hpp"

ExchangeRate::ExchangeRate(const std::string& filename)
    : ADatabase(filename, "date,exchange_rate", ",",
            std::numeric_limits<double>::infinity()) {
    std::string line = "";
    while (std::getline(ifs_, line)) {
        ++lineCount_;
        try {
            checkFormat(line);
            database_.insert(
                std::make_pair(
                    extractDate(line), extractValue(line.substr(valueIndex_))));
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    if (ifs_.bad())
        throw std::runtime_error(createErrorMessage(0, 0, "could not read"));
}

ExchangeRate::~ExchangeRate() {
}
