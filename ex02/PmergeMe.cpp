/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyoshid <akyoshid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 23:43:19 by akyoshid          #+#    #+#             */
/*   Updated: 2025/09/18 20:05:05 by akyoshid         ###   ########.fr       */
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
#include <vector>
#include <deque>
#include "PmergeMe.hpp"

PmergeMe::PmergeMe(int argc, char **argv) : compCount_(0) {
    parseArgs(argc, argv);
    deq_.assign(vec_.begin(), vec_.end());
    initCustomJacobsthal();
    displayBeforeVec();
    sortVector();
    displayAfterVec();
    // displayBeforeDeq();
    sortDeque();
    // displayAfterDeq();
    displayTimes();
    std::cout << "Number of comparisons: " << compCount_ << std::endl;
    if (!isSortedVec()) {
        std::cout << "\033[31m" << "VECTOR: NOT SORTED"
            << "\033[0m" << std::endl;
    }
    if (!isSortedDeq()) {
        std::cout << "\033[31m" << "DEQUE: NOT SORTED"
            << "\033[0m" << std::endl;
    }
}

PmergeMe::~PmergeMe() {
}

////////////
// SHARED //
////////////

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
        customJacobsthal_[i]
            = customJacobsthal_[i - 1] + 2 * customJacobsthal_[i - 2];
    }
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

////////////
// VECTOR //
////////////

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

void PmergeMe::mergeInsertionSortVector(int depth) {
    int elementSize = powerOfTwo(depth);
    if (elementSize > static_cast<int>(vec_.size()) / 2)
        return;
    int pairSize = elementSize * 2;
    std::vector<unsigned int> tmp;
    tmp.reserve(elementSize);
    for (int i = elementSize - 1;
        i + elementSize < static_cast<int>(vec_.size()); i += pairSize) {
        ++compCount_;
        if (vec_[i] > vec_[i + elementSize]) {
            // tmp.assign(
            //     vec_.begin() + i + 1, vec_.begin() + i + 1 + elementSize);
            // vec_.erase(
            //     vec_.begin() + i + 1, vec_.begin() + i + 1 + elementSize);
            // vec_.insert(vec_.begin() + i - (elementSize - 1),
            //     tmp.begin(), tmp.end());
            std::rotate(vec_.begin() + i - (elementSize - 1),
                vec_.begin() + i + 1, vec_.begin() + i + 1 + elementSize);
        }
    }
    mergeInsertionSortVector(depth + 1);
    initMainVec(elementSize, pairSize);
    initPendVec(elementSize, pairSize);
    int customJacobsthalIndex = 0;
    int groupSize;
    while (!pendVec_.empty()) {
        if (customJacobsthalIndex >= CUSTOM_JACOBSTHAL_SIZE)
            throw std::runtime_error(
                "Error: custom Jacobsthal numbers exhausted");
        groupSize = customJacobsthal_[customJacobsthalIndex];
        if (groupSize > static_cast<int>(pendVec_.size())) {
            groupSize = static_cast<int>(pendVec_.size());
        }
        for (int groupIndex = groupSize - 1; groupIndex >= 0; --groupIndex) {
            int destMainIndex = getDestMainIndexVec(groupIndex);
            int destIndex;
            if (destMainIndex < static_cast<int>(mainVec_.size())) {
                if (mainVec_[destMainIndex].pairIndex_ == NO_PAIR_INDEX) {
                    destIndex
                        = mainVec_[destMainIndex].index_ - elementSize + 1;
                } else {
                    destIndex
                        = pendVec_[mainVec_[destMainIndex].pairIndex_].index_
                            - elementSize + 1;
                }
            } else {
                destIndex
                    = pendVec_[pendVec_.size() - 1].index_ - elementSize + 1;
            }
            tmp.assign(
                vec_.begin() + pendVec_[groupIndex].index_ - elementSize + 1,
                vec_.begin() + pendVec_[groupIndex].index_ + 1);
            vec_.erase(
                vec_.begin() + pendVec_[groupIndex].index_ - elementSize + 1,
                vec_.begin() + pendVec_[groupIndex].index_ + 1);
            vec_.insert(vec_.begin() + destIndex, tmp.begin(), tmp.end());
            // std::rotate(vec_.begin() + destIndex,
            //     vec_.begin() + pendVec_[groupIndex].index_ - elementSize + 1,
            //     vec_.begin() + pendVec_[groupIndex].index_ + 1);
            fixMainVec(destMainIndex, groupIndex, elementSize);
            fixPendVec(destIndex, groupIndex, elementSize);
            t_index_set tmpMain = {destIndex + elementSize - 1, NO_PAIR_INDEX};
            mainVec_.insert(mainVec_.begin() + destMainIndex, tmpMain);
        }
        pendVec_.erase(pendVec_.begin(), pendVec_.begin() + groupSize);
        fixMainVecAfterPendErase(groupSize);
        ++customJacobsthalIndex;
    }
}

void PmergeMe::initMainVec(int elementSize, int pairSize) {
    int vecSize = static_cast<int>(vec_.size());
    mainVec_.clear();
    mainVec_.reserve(vecSize / elementSize);
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
    pendVec_.reserve(vecSize / pairSize);
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
        if (vec_[pendVec_[srcPendIndex].index_]
            < vec_[mainVec_[midMainIndex].index_]) {
            endMainIndex = midMainIndex - 1;
        } else {
            startMainIndex = midMainIndex + 1;
        }
    }
    return startMainIndex;
}

void PmergeMe::fixMainVec(
    int destMainIndex, int srcPendIndex, int elementSize) {
    int endMainIndex = pendVec_[srcPendIndex].pairIndex_;
    if (endMainIndex == NO_PAIR_INDEX) {
        endMainIndex = static_cast<int>(mainVec_.size());
    }
    for (int i = destMainIndex; i < endMainIndex; ++i) {
        mainVec_[i].index_ += elementSize;
    }
    if (pendVec_[srcPendIndex].pairIndex_ != NO_PAIR_INDEX)
        mainVec_[pendVec_[srcPendIndex].pairIndex_].pairIndex_ = NO_PAIR_INDEX;
}

void PmergeMe::fixPendVec(int destIndex, int srcPendIndex, int elementSize) {
    int beginPendIndex = static_cast<int>(pendVec_.size());
    for (int i = 0; i < static_cast<int>(pendVec_.size()); ++i) {
        if (pendVec_[i].index_ >= destIndex) {
            beginPendIndex = i;
            break;
        }
    }
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

void PmergeMe::fixMainVecAfterPendErase(int groupSize) {
    for (int i = 0; i < static_cast<int>(mainVec_.size()); ++i) {
        if (mainVec_[i].pairIndex_ != NO_PAIR_INDEX)
            mainVec_[i].pairIndex_ -= groupSize;
    }
}

void PmergeMe::displayBeforeVec() const {
    std::cout << "Before: ";
    size_t size = vec_.size();
    // for (size_t i = 0; i < size; ++i) {
    //     std::cout << vec_[i];
    //     if (i < size - 1) {
    //         std::cout << " ";
    //     }
    // }
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

void PmergeMe::displayAfterVec() const {
    std::cout << "After:  ";
    size_t size = vec_.size();
    // for (size_t i = 0; i < size; ++i) {
    //     std::cout << vec_[i];
    //     if (i < size - 1) {
    //         std::cout << " ";
    //     }
    // }
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

bool PmergeMe::isSortedVec() const {
    for (size_t i = 1; i < vec_.size(); ++i) {
        if (vec_[i - 1] > vec_[i]) {
            return false;
        }
    }
    return true;
}

///////////
// DEQUE //
///////////

void PmergeMe::sortDeque() {
    timespec start;
    timespec end;
    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0)
        throw std::runtime_error("Error: clock_gettime failed");
    mergeInsertionSortDeque(0);
    if (clock_gettime(CLOCK_MONOTONIC, &end) != 0)
        throw std::runtime_error("Error: clock_gettime failed");
    deqSortTime_ = getSortTime(start, end);
}

void PmergeMe::mergeInsertionSortDeque(int depth) {
    int elementSize = powerOfTwo(depth);
    if (elementSize > static_cast<int>(deq_.size()) / 2)
        return;
    int pairSize = elementSize * 2;
    std::deque<unsigned int> tmp;
    for (int i = elementSize - 1;
        i + elementSize < static_cast<int>(deq_.size()); i += pairSize) {
        if (deq_[i] > deq_[i + elementSize]) {
            // tmp.assign(
            //     deq_.begin() + i + 1, deq_.begin() + i + 1 + elementSize);
            // deq_.erase(
            //     deq_.begin() + i + 1, deq_.begin() + i + 1 + elementSize);
            // deq_.insert(deq_.begin() + i - (elementSize - 1),
            //     tmp.begin(), tmp.end());
            std::rotate(
                deq_.begin() + i - (elementSize - 1), deq_.begin() + i + 1,
                deq_.begin() + i + 1 + elementSize);
        }
    }
    mergeInsertionSortDeque(depth + 1);
    initMainDeq(elementSize, pairSize);
    initPendDeq(elementSize, pairSize);
    int customJacobsthalIndex = 0;
    int groupSize;
    while (!pendDeq_.empty()) {
        if (customJacobsthalIndex >= CUSTOM_JACOBSTHAL_SIZE)
            throw std::runtime_error(
                "Error: custom Jacobsthal numbers exhausted");
        groupSize = customJacobsthal_[customJacobsthalIndex];
        if (groupSize > static_cast<int>(pendDeq_.size())) {
            groupSize = static_cast<int>(pendDeq_.size());
        }
        for (int groupIndex = groupSize - 1; groupIndex >= 0; --groupIndex) {
            int destMainIndex = getDestMainIndexDeq(groupIndex);
            int destIndex;
            if (destMainIndex < static_cast<int>(mainDeq_.size())) {
                if (mainDeq_[destMainIndex].pairIndex_ == NO_PAIR_INDEX) {
                    destIndex
                        = mainDeq_[destMainIndex].index_ - elementSize + 1;
                } else {
                    destIndex
                        = pendDeq_[mainDeq_[destMainIndex].pairIndex_].index_
                            - elementSize + 1;
                }
            } else {
                destIndex
                    = pendDeq_[pendDeq_.size() - 1].index_ - elementSize + 1;
            }
            tmp.assign(
                deq_.begin() + pendDeq_[groupIndex].index_ - elementSize + 1,
                deq_.begin() + pendDeq_[groupIndex].index_ + 1);
            deq_.erase(
                deq_.begin() + pendDeq_[groupIndex].index_ - elementSize + 1,
                deq_.begin() + pendDeq_[groupIndex].index_ + 1);
            deq_.insert(deq_.begin() + destIndex, tmp.begin(), tmp.end());
            // std::rotate(deq_.begin() + destIndex,
            //     deq_.begin() + pendDeq_[groupIndex].index_ - elementSize + 1,
            //     deq_.begin() + pendDeq_[groupIndex].index_ + 1);
            fixMainDeq(destMainIndex, groupIndex, elementSize);
            fixPendDeq(destIndex, groupIndex, elementSize);
            t_index_set tmpMain = {destIndex + elementSize - 1, NO_PAIR_INDEX};
            mainDeq_.insert(mainDeq_.begin() + destMainIndex, tmpMain);
        }
        pendDeq_.erase(pendDeq_.begin(), pendDeq_.begin() + groupSize);
        fixMainDeqAfterPendErase(groupSize);
        ++customJacobsthalIndex;
    }
}

void PmergeMe::initMainDeq(int elementSize, int pairSize) {
    int deqSize = static_cast<int>(deq_.size());
    mainDeq_.clear();
    if (elementSize <= deqSize) {
        t_index_set tmp = {elementSize - 1, NO_PAIR_INDEX};
        mainDeq_.push_back(tmp);
    }
    if (pairSize <= deqSize) {
        t_index_set tmp = {pairSize - 1, NO_PAIR_INDEX};
        mainDeq_.push_back(tmp);
    }
    int pairPendIndex = 0;
    for (int i = pairSize * 2 - 1; i < deqSize; i += pairSize) {
        t_index_set tmp = {i, pairPendIndex};
        mainDeq_.push_back(tmp);
        ++pairPendIndex;
    }
}

void PmergeMe::initPendDeq(int elementSize, int pairSize) {
    int deqSize = static_cast<int>(deq_.size());
    pendDeq_.clear();
    int pairMainIndex = 2;
    for (int i = pairSize + elementSize - 1; i < deqSize; i += pairSize) {
        if (i + elementSize < deqSize) {
            t_index_set tmp = {i, pairMainIndex};
            pendDeq_.push_back(tmp);
        } else {
            t_index_set tmp = {i, NO_PAIR_INDEX};
            pendDeq_.push_back(tmp);
        }
        ++pairMainIndex;
    }
}

int PmergeMe::getDestMainIndexDeq(int srcPendIndex) {
    int startMainIndex = 0;
    int endMainIndex;
    if (pendDeq_[srcPendIndex].pairIndex_ == NO_PAIR_INDEX) {
        endMainIndex = static_cast<int>(mainDeq_.size()) - 1;
    } else {
        endMainIndex = pendDeq_[srcPendIndex].pairIndex_ - 1;
    }
    while (startMainIndex <= endMainIndex) {
        int midMainIndex = startMainIndex + (endMainIndex - startMainIndex) / 2;
        if (deq_[pendDeq_[srcPendIndex].index_]
            < deq_[mainDeq_[midMainIndex].index_]) {
            endMainIndex = midMainIndex - 1;
        } else {
            startMainIndex = midMainIndex + 1;
        }
    }
    return startMainIndex;
}

void PmergeMe::fixMainDeq(
    int destMainIndex, int srcPendIndex, int elementSize) {
    int endMainIndex = pendDeq_[srcPendIndex].pairIndex_;
    if (endMainIndex == NO_PAIR_INDEX) {
        endMainIndex = static_cast<int>(mainDeq_.size());
    }
    for (int i = destMainIndex; i < endMainIndex; ++i) {
        mainDeq_[i].index_ += elementSize;
    }
    if (pendDeq_[srcPendIndex].pairIndex_ != NO_PAIR_INDEX)
        mainDeq_[pendDeq_[srcPendIndex].pairIndex_].pairIndex_ = NO_PAIR_INDEX;
}

void PmergeMe::fixPendDeq(int destIndex, int srcPendIndex, int elementSize) {
    int beginPendIndex = static_cast<int>(pendDeq_.size());
    for (int i = 0; i < static_cast<int>(pendDeq_.size()); ++i) {
        if (pendDeq_[i].index_ >= destIndex) {
            beginPendIndex = i;
            break;
        }
    }
    for (int i = beginPendIndex; i < static_cast<int>(pendDeq_.size()); ++i) {
        if (i < srcPendIndex) {
            pendDeq_[i].index_ += elementSize;
        }
        if (pendDeq_[i].pairIndex_ != NO_PAIR_INDEX) {
            ++(pendDeq_[i].pairIndex_);
        }
    }
    pendDeq_[srcPendIndex].pairIndex_ = NO_PAIR_INDEX;
}

void PmergeMe::fixMainDeqAfterPendErase(int groupSize) {
    for (int i = 0; i < static_cast<int>(mainDeq_.size()); ++i) {
        if (mainDeq_[i].pairIndex_ != NO_PAIR_INDEX)
            mainDeq_[i].pairIndex_ -= groupSize;
    }
}

void PmergeMe::displayBeforeDeq() const {
    std::cout << "Before: ";
    size_t size = deq_.size();
    // for (size_t i = 0; i < size; ++i) {
    //     std::cout << deq_[i];
    //     if (i < size - 1) {
    //         std::cout << " ";
    //     }
    // }
    if (size > 5) {
        for (size_t i = 0; i < 4; ++i) {
            std::cout << deq_[i] << " ";
        }
        std::cout << "[...]";
    } else {
        for (size_t i = 0; i < size; ++i) {
            std::cout << deq_[i];
            if (i < size - 1) {
                std::cout << " ";
            }
        }
    }
    std::cout << std::endl;
}

void PmergeMe::displayAfterDeq() const {
    std::cout << "After:  ";
    size_t size = deq_.size();
    // for (size_t i = 0; i < size; ++i) {
    //     std::cout << deq_[i];
    //     if (i < size - 1) {
    //         std::cout << " ";
    //     }
    // }
    if (size > 5) {
        for (size_t i = 0; i < 4; ++i) {
            std::cout << deq_[i] << " ";
        }
        std::cout << "[...]";
    } else {
        for (size_t i = 0; i < size; ++i) {
            std::cout << deq_[i];
            if (i < size - 1) {
                std::cout << " ";
            }
        }
    }
    std::cout << std::endl;
}

void PmergeMe::displayMainDeq() const {
    for (size_t i = 0; i < mainDeq_.size(); ++i) {
        std::cout << "{" << mainDeq_[i].index_ << ",";
        if (mainDeq_[i].pairIndex_ != NO_PAIR_INDEX) {
            std::cout << mainDeq_[i].pairIndex_ << "} ";
        } else {
            std::cout << "-} ";
        }
    }
    std::cout << std::endl;
}

void PmergeMe::displayPendDeq() const {
    for (size_t i = 0; i < pendDeq_.size(); ++i) {
        std::cout << "{" << pendDeq_[i].index_ << ",";
        if (pendDeq_[i].pairIndex_ != NO_PAIR_INDEX) {
            std::cout << pendDeq_[i].pairIndex_ << "} ";
        } else {
            std::cout << "-} ";
        }
    }
    std::cout << std::endl;
}

bool PmergeMe::isSortedDeq() const {
    for (size_t i = 1; i < deq_.size(); ++i) {
        if (deq_[i - 1] > deq_[i]) {
            return false;
        }
    }
    return true;
}
