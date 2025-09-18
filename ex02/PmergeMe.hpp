/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyoshid <akyoshid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 23:43:11 by akyoshid          #+#    #+#             */
/*   Updated: 2025/09/18 16:23:12 by akyoshid         ###   ########.fr       */
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
    // shared
    typedef struct {
        int index_;
        int pairIndex_;
    } t_index_set;
    int customJacobsthal_[CUSTOM_JACOBSTHAL_SIZE];
    void parseArgs(int argc, char **argv);
    unsigned int parseNumber(const std::string& num);
    int powerOfTwo(int n) const;
    void initCustomJacobsthal();
    void displayTimes() const;
    double getSortTime(const timespec& start, const timespec& end) const;
    // vector
    std::vector<unsigned int> vec_;
    std::vector<t_index_set> mainVec_;
    std::vector<t_index_set> pendVec_;
    double vecSortTime_;
    size_t compCount_;
    void sortVector();
    void mergeInsertionSortVector(int depth);
    void initMainVec(int elementSize, int pairSize);
    void initPendVec(int elementSize, int pairSize);
    int getDestMainIndexVec(int srcPendIndex);
    void fixMainVec(int destMainIndex, int srcPendIndex, int elementSize);
    void fixPendVec(int destIndex, int srcPendIndex, int elementSize);
    void fixMainVecAfterPendErase(int groupSize);
    void displayBeforeVec() const;
    void displayAfterVec() const;
    void displayMainVec() const;
    void displayPendVec() const;
    bool isSortedVec() const;
    // deque
    std::deque<unsigned int> deq_;
    std::deque<t_index_set> mainDeq_;
    std::deque<t_index_set> pendDeq_;
    double deqSortTime_;
    void sortDeque();
    void mergeInsertionSortDeque(int depth);
    void initMainDeq(int elementSize, int pairSize);
    void initPendDeq(int elementSize, int pairSize);
    int getDestMainIndexDeq(int srcPendIndex);
    void fixMainDeq(int destMainIndex, int srcPendIndex, int elementSize);
    void fixPendDeq(int destIndex, int srcPendIndex, int elementSize);
    void fixMainDeqAfterPendErase(int groupSize);
    void displayMainDeq() const;
    void displayPendDeq() const;
    bool isSortedDeq() const;
    // delete
    PmergeMe(); // = delete
    PmergeMe(const PmergeMe& src); // = delete
    PmergeMe& operator=(const PmergeMe& src); // = delete
};

#endif
