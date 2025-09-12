/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyoshid <akyoshid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 05:41:02 by akyoshid          #+#    #+#             */
/*   Updated: 2025/09/12 16:22:42 by akyoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stdexcept>
#include "BitcoinExchange.hpp"
#include "ExchangeRate.hpp"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Error: could not open file." << std::endl;
        return 1;
    }
    try {
        ExchangeRate d1("data.csv");
        BitcoinExchange d2(argv[1], d1);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
