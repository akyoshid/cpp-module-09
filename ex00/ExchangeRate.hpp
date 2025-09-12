/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExchangeRate.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyoshid <akyoshid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:54:52 by akyoshid          #+#    #+#             */
/*   Updated: 2025/09/12 12:48:10 by akyoshid         ###   ########.fr       */
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
    static const size_t dateLen_;
    std::string getHeader() const;
    std::string getSeparator() const;
    size_t getSeparatorLen() const;
    size_t getValueIndex() const;
    double getValueMax() const;
    void checkFormat(const std::string& line) const;
    void checkLineLen(const std::string& line) const;
    void checkDateFormat(const std::string& line) const;
    void checkSeparatorFormat(const std::string& separator) const;
    void checkValueFormat(const std::string& value) const;
    int extractDate(const std::string& line) const;
    double extractValue(const std::string& value) const;
    std::string createErrorMessage(
        size_t line, size_t column, const std::string& msg) const;
    ExchangeRate(); // = delete
    ExchangeRate(const ExchangeRate& src); // = delete
    ExchangeRate& operator=(const ExchangeRate& src); // = delete
};

#endif
