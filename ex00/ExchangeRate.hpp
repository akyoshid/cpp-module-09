/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExchangeRate.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyoshid <akyoshid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:54:52 by akyoshid          #+#    #+#             */
/*   Updated: 2025/09/12 13:36:06 by akyoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EX00_EXCHANGERATE_HPP_
# define EX00_EXCHANGERATE_HPP_

# include <string>
# include "ADatabase.hpp"

class ExchangeRate : public ADatabase {
 public:
    ExchangeRate(const std::string& filename);
    ~ExchangeRate();
 private:
    ExchangeRate(); // = delete
    ExchangeRate(const ExchangeRate& src); // = delete
    ExchangeRate& operator=(const ExchangeRate& src); // = delete
};

#endif
