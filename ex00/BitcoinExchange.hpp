/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyoshid <akyoshid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 05:41:11 by akyoshid          #+#    #+#             */
/*   Updated: 2025/09/12 16:21:16 by akyoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EX00_BITCOINEXCHANGE_HPP_
# define EX00_BITCOINEXCHANGE_HPP_

# include <string>
# include "ADatabase.hpp"
# include "ExchangeRate.hpp"

class BitcoinExchange : public ADatabase {
 public:
    BitcoinExchange(const std::string& filename, const ExchangeRate& rate);
    ~BitcoinExchange();
 private:
    static void exchange(int date, double value, const ExchangeRate& rate);
    BitcoinExchange(); // = delete
    BitcoinExchange(const BitcoinExchange& src); // = delete
    BitcoinExchange& operator=(const BitcoinExchange& src); // = delete
};

#endif
