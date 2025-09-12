/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyoshid <akyoshid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:17:57 by akyoshid          #+#    #+#             */
/*   Updated: 2025/09/12 23:01:42 by akyoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EX01_RPN_HPP_
# define EX01_RPN_HPP_

# include <string>
# include <stack>

enum CalcType {
    ADD,
    SUB,
    MUL,
    DIV
};

class RPN {
 public:
    RPN();
    ~RPN();
    int exec(const std::string& expr);
 private:
    std::stack<int> stack_;
    void calc(CalcType type, size_t pos);
    void add(int op1, int op2, size_t pos);
    void sub(int op1, int op2, size_t pos);
    void mul(int op1, int op2, size_t pos);
    void div(int op1, int op2, size_t pos);
    void handleError(size_t pos, const std::string& mes);
    RPN(const RPN& src); // = delete
    RPN& operator=(const RPN& src); // = delete
};

#endif
