/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyoshid <akyoshid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 23:43:11 by akyoshid          #+#    #+#             */
/*   Updated: 2025/09/18 13:55:22 by akyoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EX02_PMERGEME_HPP_
# define EX02_PMERGEME_HPP_

# include <vector>
# include <deque>

# define CUSTOM_JACOBSTHAL_SIZE 15
# define NO_PAIR_INDEX -1

class PmergeMe {
 public:
    PmergeMe(int argc, char **argv);
    ~PmergeMe();
 private:
    std::vector<unsigned int> vec_;
    std::deque<unsigned int> deq_;
    typedef struct {
        int index_;
        int pairIndex_;
    } t_index_set;
    std::vector<t_index_set> mainVec_;
    std::vector<t_index_set> pendVec_;
    int customJacobsthal_[CUSTOM_JACOBSTHAL_SIZE];
    size_t compCount_;
    double vecSortTime_;
    double deqSortTime_;
    void parseArgs(int argc, char **argv);
    unsigned int parseNumber(const std::string& num);
    void sortVector();
    void sortDeque();
    void mergeInsertionSortVector(int depth);
    void swapElementVector(int i1, int i2, int elementSize);
    void initMainVec(int elementSize, int pairSize);
    void initPendVec(int elementSize, int pairSize);
    int getDestMainIndexVec(int srcPendIndex);
    int binarySearchVec(int value, int startMainIndex, int endMainIndex);
    void fixMainVec(int destMainIndex, int srcPendIndex, int elementSize);
    void fixPendVec(int destIndex, int srcPendIndex, int elementSize);
    void fixMainVecAfterPendErase(int groupSize);
    void mergeInsertionSortDeque(int depth);
    int powerOfTwo(int n) const;
    void initCustomJacobsthal();
    void displayBefore() const;
    void displayAfter() const;
    void displayTimes() const;
    double getSortTime(const timespec& start, const timespec& end) const;
    void displayMainVec() const;
    void displayPendVec() const;
    bool isSortedVec() const;
    PmergeMe(); // = delete
    PmergeMe(const PmergeMe& src); // = delete
    PmergeMe& operator=(const PmergeMe& src); // = delete
};

#endif
