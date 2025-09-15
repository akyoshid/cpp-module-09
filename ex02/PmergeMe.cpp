/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyoshid <akyoshid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 23:43:19 by akyoshid          #+#    #+#             */
/*   Updated: 2025/09/15 10:51:12 by akyoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <stdexcept>
#include <cerrno>
#include <cstdlib>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <cctype>
#include <time.h>
#include "PmergeMe.hpp"

PmergeMe::PmergeMe(int argc, char **argv) {
    parseArgs(argc, argv);
    deq_.assign(vec_.begin(), vec_.end());
    displayBefore();
    sortVector();
    sortDeque();
    displayAfter();
    displayTimes();
}

PmergeMe::~PmergeMe() {
}

void PmergeMe::parseArgs(int argc, char **argv) {
    if (argc < 2) {
        throw std::runtime_error("Usage: ./PmergeMe <positive integer> ...");
    } else if (argc > 100000) {
        throw std::runtime_error("Error: too many numbers");
    }
    vec_.reserve(argc - 1);
    for (int i = 1; i < argc; ++i) {
        vec_.push_back(parseNumber(argv[i]));
    }
}

unsigned int PmergeMe::parseNumber(const std::string& num) {
    if (num.empty()) {
        throw std::runtime_error("Error: empty string");
    } else if (num[0] == '-') {
        throw std::runtime_error("Error: negative number");
    }
    for (size_t i = 0; i < num.length(); ++i) {
        if (i == 0 && num[i] == '+') {
            continue;
        } else if (std::isdigit(static_cast<unsigned char>(num[i]))) {
            continue;
        } else {
            throw std::runtime_error("Error: invalid character");
        }
    }
    char *endptr;
    errno = 0;
    unsigned long ret = std::strtoul(num.c_str(), &endptr, 10);
    if (errno == ERANGE || ret > std::numeric_limits<unsigned int>::max()) {
        throw std::runtime_error("Error: too large number");
    } else if (*endptr != '\0') {
        throw std::runtime_error("Error: invalid character");
    } else if (ret == 0) {
        throw std::runtime_error("Error: invalid integer 0");
    }
    return static_cast<unsigned int>(ret);
}

void PmergeMe::sortVector() {
    timespec start;
    timespec end;
    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0)
        throw std::runtime_error("Error: clock_gettime failed");
    std::sort(vec_.begin(), vec_.end());
    if (clock_gettime(CLOCK_MONOTONIC, &end) != 0)
        throw std::runtime_error("Error: clock_gettime failed");
    vecSortTime_ = getSortTime(start, end);
}

void PmergeMe::sortDeque() {
    timespec start;
    timespec end;
    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0)
        throw std::runtime_error("Error: clock_gettime failed");
    std::sort(deq_.begin(), deq_.end());
    if (clock_gettime(CLOCK_MONOTONIC, &end) != 0)
        throw std::runtime_error("Error: clock_gettime failed");
    deqSortTime_ = getSortTime(start, end);
}

void PmergeMe::displayBefore() const {
    std::cout << "Before: ";
    size_t size = vec_.size();
    if (size > 5) {
        for (size_t i = 0; i < 4; ++i) {
            std::cout << vec_[i] << " ";
        }
        std::cout << "[...]";
    } else {
        for (size_t i = 0; i < size; ++i) {
            std::cout << vec_[i];
            if (i < size - 1) {
                std::cout << " ";
            }
        }
    }
    std::cout << std::endl;
}

int PmergeMe::powerOfTwo(int n) const {
    if (n >= (static_cast<int>(sizeof(int)) * 8) - 1) {
        throw std::runtime_error("Error: powerOfTwo failed due to overflow");
    }
    return 1 << n;
}

// Returns the n-th Jacobsthal number using the closed formula:
// J(n) = (2^n - (-1)^n) / 3
// For even n: J(n) = (2^n - 1) / 3
// For odd n:  J(n) = (2^n + 1) / 3
int PmergeMe::getJacobsthalNumber(int n) const {
    if (n % 2 == 0) {
        return (powerOfTwo(n) - 1) / 3;
    } else {
        return (powerOfTwo(n) + 1) / 3;
    }
}

void PmergeMe::displayAfter() const {
    std::cout << "After: ";
    size_t size = vec_.size();
    if (size > 5) {
        for (size_t i = 0; i < 4; ++i) {
            std::cout << vec_[i] << " ";
        }
        std::cout << "[...]";
    } else {
        for (size_t i = 0; i < size; ++i) {
            std::cout << vec_[i];
            if (i < size - 1) {
                std::cout << " ";
            }
        }
    }
    std::cout << std::endl;
}

void PmergeMe::displayTimes() const {
    size_t size = vec_.size();
    std::cout << "Time to process a range of " << size 
              << " elements with std::vector : " 
              << std::fixed << std::setprecision(5) 
              << vecSortTime_ * 1000000 << " us" << std::endl;
    std::cout << "Time to process a range of " << size 
              << " elements with std::deque  : " 
              << std::fixed << std::setprecision(5) 
              << deqSortTime_ * 1000000 << " us" << std::endl;
}

double PmergeMe::getSortTime(const timespec& start, const timespec& end) const {
    long sec  = end.tv_sec  - start.tv_sec;
    long nsec = end.tv_nsec - start.tv_nsec;
    if (nsec < 0) {
        --sec;
        nsec += 1000000000L;
    }
    return static_cast<double>(sec) + static_cast<double>(nsec) / 1000000000.0;
}
