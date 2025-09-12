/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADatabase.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyoshid <akyoshid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 19:41:18 by akyoshid          #+#    #+#             */
/*   Updated: 2025/09/12 19:05:21 by akyoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EX00_ADATABASE_HPP_
# define EX00_ADATABASE_HPP_

# include <string>
# include <map>
#include <fstream>

class ADatabase {
 public:
    ADatabase(
        const std::string& filename, const std::string& header,
        const std::string& separator, double valueMax);
    virtual ~ADatabase();
    double searchLE(int key) const;
 protected:
    std::ifstream ifs_;
    std::map<int, double> database_;
    std::string filename_;
    std::string header_;
    std::string separator_;
    size_t valueIndex_;
    double valueMax_;
    int lineCount_;
    static const size_t dateLen_;
    void checkFormat(const std::string& line) const;
    int extractDate(const std::string& line) const;
    double extractValue(const std::string& value) const;
    std::string createErrorMessage(
        size_t line, size_t column, const std::string& msg) const;
    static std::string createDateStr(int date);
 private:
    void checkLineLen(const std::string& line) const;
    void checkDateFormat(const std::string& line) const;
    void checkSeparatorFormat(const std::string& separator) const;
    void checkValueFormat(const std::string& value) const;
    ADatabase(); // = delete
    ADatabase(const ADatabase& src); // = delete
    ADatabase& operator=(const ADatabase& src); // = delete
};

#endif
