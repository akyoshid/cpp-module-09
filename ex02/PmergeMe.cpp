/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyoshid <akyoshid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 23:43:19 by akyoshid          #+#    #+#             */
/*   Updated: 2025/09/17 18:50:28 by akyoshid         ###   ########.fr       */
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

PmergeMe::PmergeMe(int argc, char **argv) : compCount_(0) {
    parseArgs(argc, argv);
    deq_.assign(vec_.begin(), vec_.end());
    initCustomJacobsthal();
    displayBefore();
    sortVector();
    displayAfter();
    sortDeque();
    displayTimes();
    std::cout << compCount_ << std::endl;
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
    // } else if (ret == 0) {
    //     throw std::runtime_error("Error: invalid integer 0");
    }
    return static_cast<unsigned int>(ret);
}

void PmergeMe::sortVector() {
    timespec start;
    timespec end;
    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0)
        throw std::runtime_error("Error: clock_gettime failed");
    mergeInsertionSortVector(0);
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

void PmergeMe::mergeInsertionSortVector(int depth) {
    int elementSize = powerOfTwo(depth);
    if (elementSize > static_cast<int>(vec_.size()) / 2)
        return ;
    int pairSize = elementSize * 2;
    for (int i = elementSize - 1;
        i + elementSize < static_cast<int>(vec_.size()); i += pairSize) {
        if (vec_[i] > vec_[i + elementSize]) {
            swapElementVector(i, i + elementSize, elementSize);
        }
        ++compCount_;
    }
    mergeInsertionSortVector(depth + 1);
    // if (depth >= 0) { //
    // std::cout << "🔥depth: " << depth << std::endl; //
    // displayBefore(); //
    initMainVec(elementSize, pairSize);
    initPendVec(elementSize, pairSize);
    // displayMainVec(); //
    // displayPendVec(); //
    int customJacobsthalIndex = 0;
    int groupSize;
    while (!pendVec_.empty()) {
        if (customJacobsthalIndex >= CUSTOM_JACOBSTHAL_SIZE)
            throw std::runtime_error("Error: custom Jacobsthal numbers exhausted");
        groupSize = customJacobsthal_[customJacobsthalIndex];
        if (groupSize > static_cast<int>(pendVec_.size())) {
            groupSize = static_cast<int>(pendVec_.size());
        }
        for (int groupIndex = groupSize - 1; groupIndex >= 0; --groupIndex) {
            int destMainIndex = getDestMainIndexVec(groupIndex);
            int destIndex;
            if (mainVec_[destMainIndex].pairIndex_ == NO_PAIR_INDEX) {
                destIndex = mainVec_[destMainIndex].index_ - elementSize + 1;
            } else {
                destIndex = pendVec_[mainVec_[destMainIndex].pairIndex_].index_ - elementSize + 1;
            }
            // std::cout << "srcPendIndex(groupIndex): " << groupIndex << std::endl; //
            // std::cout << "destMainIndex: " << destMainIndex << std::endl; //
            // std::cout << "destIndex: " << destIndex << std::endl; //
            std::vector<unsigned int> tmp(vec_.begin() + pendVec_[groupIndex].index_ - elementSize + 1, vec_.begin() + pendVec_[groupIndex].index_ + 1);
            vec_.erase(vec_.begin() + pendVec_[groupIndex].index_ - elementSize + 1, vec_.begin() + pendVec_[groupIndex].index_ + 1);
            vec_.insert(vec_.begin() + destIndex, tmp.begin(), tmp.end());
            // displayAfter(); //
            fixMainVec(destMainIndex, groupIndex, elementSize);
            fixPendVec(destMainIndex, groupIndex, elementSize);
            t_index_set tmpMain = {destIndex + elementSize - 1, NO_PAIR_INDEX};
            mainVec_.insert(mainVec_.begin() + destMainIndex, tmpMain);
            // displayMainVec(); //
            // displayPendVec(); //
        }
        pendVec_.erase(pendVec_.begin(), pendVec_.begin() + groupSize);
        ++customJacobsthalIndex;
    }
    // } //
}

void PmergeMe::initMainVec(int elementSize, int pairSize) {
    int vecSize = static_cast<int>(vec_.size());
    mainVec_.clear();
    if (elementSize <= vecSize) {
        t_index_set tmp = {elementSize - 1, NO_PAIR_INDEX};
        mainVec_.push_back(tmp);
    }
    if (pairSize <= vecSize) {
        t_index_set tmp = {pairSize - 1, NO_PAIR_INDEX};
        mainVec_.push_back(tmp);
    }
    int pairPendIndex = 0;
    for (int i = pairSize * 2 - 1; i < vecSize; i += pairSize) {
        t_index_set tmp = {i, pairPendIndex};
        mainVec_.push_back(tmp);
        ++pairPendIndex;
    }
}

void PmergeMe::initPendVec(int elementSize, int pairSize) {
    int vecSize = static_cast<int>(vec_.size());
    pendVec_.clear();
    int pairMainIndex = 2;
    for (int i = pairSize + elementSize - 1; i < vecSize; i += pairSize) {
        if (i + elementSize < vecSize) {
            t_index_set tmp = {i, pairMainIndex};
            pendVec_.push_back(tmp);
        } else {
            t_index_set tmp = {i, NO_PAIR_INDEX};
            pendVec_.push_back(tmp);
        }
        ++pairMainIndex;
    }
}

int PmergeMe::getDestMainIndexVec(int srcPendIndex) {
    int startMainIndex = 0;
    int endMainIndex;
    if (pendVec_[srcPendIndex].pairIndex_ == NO_PAIR_INDEX) {
        endMainIndex = static_cast<int>(mainVec_.size()) - 1;
    } else {
        endMainIndex = pendVec_[srcPendIndex].pairIndex_ - 1;
    }
    while (startMainIndex <= endMainIndex) {
        int midMainIndex = startMainIndex + (endMainIndex - startMainIndex) / 2;
        ++compCount_;
        // std::cout << "getDestMainIndexVec: startMainIndex: " << startMainIndex << std::endl; //
        // std::cout << "getDestMainIndexVec: midMainIndex: " << midMainIndex << std::endl; //
        // std::cout << "getDestMainIndexVec: endMainIndex: " << endMainIndex << std::endl; //
        // std::cout << "getDestMainIndexVec: pendVec_[srcPendIndex].index_: " << pendVec_[srcPendIndex].index_ << std::endl; //
        // std::cout << "getDestMainIndexVec: mainVec_[midMainIndex].index_: " << mainVec_[midMainIndex].index_ << std::endl; //
        // std::cout << "getDestMainIndexVec: vec_[pendVec_[srcPendIndex].index_]: " << vec_[pendVec_[srcPendIndex].index_] << std::endl; //
        // std::cout << "getDestMainIndexVec: vec_[mainVec_[midMainIndex].index_]: " << vec_[mainVec_[midMainIndex].index_] << std::endl; //
        if (vec_[pendVec_[srcPendIndex].index_]
            < vec_[mainVec_[midMainIndex].index_]) {
            endMainIndex = midMainIndex - 1;
        } else {
            startMainIndex = midMainIndex + 1;
        }
    }
    return startMainIndex;
}

// int PmergeMe::getDestMainIndexVec(int srcPendIndex) {
//     int endMainIndex = pendVec_[srcPendIndex].pairIndex_ - 1;
//     int value = vec_[pendVec_[srcPendIndex].index_];
//     return binarySearchVec(value, 0, endMainIndex);
// }

// int PmergeMe::binarySearchVec(int value, int startMainIndex, int endMainIndex) {
//     while (startMainIndex <= endMainIndex) {
//         int midMainIndex = startMainIndex + (endMainIndex - startMainIndex) / 2;
//         ++compCount_;
//         if (value < vec_[mainVec_[midMainIndex].index_]) {
//             endMainIndex = midMainIndex - 1;
//         } else {
//             startMainIndex = midMainIndex + 1;
//         }
//     }
//     return startMainIndex;
// }

void PmergeMe::fixMainVec(int destMainIndex, int srcPendIndex, int elementSize) {
    int endMainIndex = pendVec_[srcPendIndex].pairIndex_;
    if (endMainIndex == NO_PAIR_INDEX) {
        endMainIndex = static_cast<int>(mainVec_.size());
    }
    // std::cout << "endMainIndex: " << endMainIndex << std::endl; //
    for (int i = destMainIndex; i < endMainIndex; ++i) {
        mainVec_[i].index_ += elementSize;
    }
    // if (pendVec_[srcPendIndex].pairIndex_ != NO_PAIR_INDEX)
    //     mainVec_[pendVec_[srcPendIndex].pairIndex_].pairIndex_ = NO_PAIR_INDEX;
}

void PmergeMe::fixPendVec(int destMainIndex, int srcPendIndex, int elementSize) {
    int beginPendIndex = mainVec_[destMainIndex].pairIndex_;
    if (beginPendIndex == NO_PAIR_INDEX) {
        beginPendIndex = 0;
    }
    if (pendVec_[srcPendIndex].pairIndex_ != NO_PAIR_INDEX)
        mainVec_[pendVec_[srcPendIndex].pairIndex_].pairIndex_ = NO_PAIR_INDEX;
    for (int i = beginPendIndex; i < static_cast<int>(pendVec_.size()); ++i) {
        if (i < srcPendIndex) {
            pendVec_[i].index_ += elementSize;
        }
        if (pendVec_[i].pairIndex_ != NO_PAIR_INDEX) {
            ++(pendVec_[i].pairIndex_);
        }
    }
    pendVec_[srcPendIndex].pairIndex_ = NO_PAIR_INDEX;
}

void PmergeMe::swapElementVector(int i1, int i2, int elementSize) {
    for (int i = 0; i < elementSize; ++i) {
        unsigned int tmp;
        tmp = vec_[i1 - i];
        vec_[i1 - i] = vec_[i2 - i];
        vec_[i2 - i] = tmp;
    }
}

int PmergeMe::powerOfTwo(int n) const {
    if (n >= (static_cast<int>(sizeof(int)) * 8) - 1) {
        throw std::runtime_error("Error: powerOfTwo failed due to overflow");
    }
    return 1 << n;
}

void PmergeMe::initCustomJacobsthal() {
    customJacobsthal_[0] = 2;
    customJacobsthal_[1] = 2;
    for (int i = 2; i < CUSTOM_JACOBSTHAL_SIZE; ++i) {
        customJacobsthal_[i] = customJacobsthal_[i - 1] + 2 * customJacobsthal_[i - 2];
    }
}

void PmergeMe::displayBefore() const {
    std::cout << "Before: ";
    size_t size = vec_.size();
    for (size_t i = 0; i < size; ++i) {
        std::cout << vec_[i];
        if (i < size - 1) {
            std::cout << " ";
        }
    }
    // if (size > 5) {
    //     for (size_t i = 0; i < 4; ++i) {
    //         std::cout << vec_[i] << " ";
    //     }
    //     std::cout << "[...]";
    // } else {
    //     for (size_t i = 0; i < size; ++i) {
    //         std::cout << vec_[i];
    //         if (i < size - 1) {
    //             std::cout << " ";
    //         }
    //     }
    // }
    std::cout << std::endl;
}

void PmergeMe::displayAfter() const {
    std::cout << "After: ";
    size_t size = vec_.size();
    for (size_t i = 0; i < size; ++i) {
        std::cout << vec_[i];
        if (i < size - 1) {
            std::cout << " ";
        }
    }
    // if (size > 5) {
    //     for (size_t i = 0; i < 4; ++i) {
    //         std::cout << vec_[i] << " ";
    //     }
    //     std::cout << "[...]";
    // } else {
    //     for (size_t i = 0; i < size; ++i) {
    //         std::cout << vec_[i];
    //         if (i < size - 1) {
    //             std::cout << " ";
    //         }
    //     }
    // }
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

void PmergeMe::displayMainVec() const {
    for (size_t i = 0; i < mainVec_.size(); ++i) {
        std::cout << "{" << mainVec_[i].index_ << ",";
        if (mainVec_[i].pairIndex_ != NO_PAIR_INDEX) {
            std::cout << mainVec_[i].pairIndex_ << "} ";
        } else {
            std::cout << "-} ";
        }
    }
    std::cout << std::endl;
}

void PmergeMe::displayPendVec() const {
    for (size_t i = 0; i < pendVec_.size(); ++i) {
        std::cout << "{" << pendVec_[i].index_ << ",";
        if (pendVec_[i].pairIndex_ != NO_PAIR_INDEX) {
            std::cout << pendVec_[i].pairIndex_ << "} ";
        } else {
            std::cout << "-} ";
        }
    }
    std::cout << std::endl;
}
