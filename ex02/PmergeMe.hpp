/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyoshid <akyoshid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 23:43:11 by akyoshid          #+#    #+#             */
/*   Updated: 2025/09/15 10:49:54 by akyoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EX02_PMERGEME_HPP_
# define EX02_PMERGEME_HPP_

# include <vector>
# include <deque>

class PmergeMe {
 public:
    PmergeMe(int argc, char **argv);
    ~PmergeMe();
 private:
    std::vector<unsigned int> vec_;
    std::deque<unsigned int> deq_;
    double vecSortTime_;
    double deqSortTime_;
    void parseArgs(int argc, char **argv);
    unsigned int parseNumber(const std::string& num);
    void sortVector();
    void sortDeque();
    void displayBefore() const;
    void displayAfter() const;
    void displayTimes() const;
    double getSortTime(const timespec& start, const timespec& end) const;
    PmergeMe(); // = delete
    PmergeMe(const PmergeMe& src); // = delete
    PmergeMe& operator=(const PmergeMe& src); // = delete
};

#endif
