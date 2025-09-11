/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExchangeRate.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyoshid <akyoshid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:54:52 by akyoshid          #+#    #+#             */
/*   Updated: 2025/09/11 19:29:19 by akyoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EX00_EXCHANGERATE_HPP_
# define EX00_EXCHANGERATE_HPP_

# include <string>
# include <map>

class ExchangeRate {
 public:
    ExchangeRate(const std::string& filename);
    ~ExchangeRate();
 private:
    std::map<int, double> database_;
    std::string filename_;
    int lineCount_;
    void checkFormat(const std::string& line) const;
    int extractDate(const std::string& line) const;
    double extractRate(const std::string& line) const;
    std::string createErrorMessage(
        int line, int column, const std::string& msg) const;
    ExchangeRate(); // = delete
    ExchangeRate(const ExchangeRate& src); // = delete
    ExchangeRate& operator=(const ExchangeRate& src); // = delete
};

#endif
