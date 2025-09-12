/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyoshid <akyoshid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:18:03 by akyoshid          #+#    #+#             */
/*   Updated: 2025/09/12 23:37:51 by akyoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>
#include <limits>
#include <ctype.h>
#include "RPN.hpp"
#include "utils.hpp"

RPN::RPN() {
}

RPN::~RPN() {
}

int RPN::exec(const std::string& expr) {
    int ret = 0;
    bool spaceFlag = false;
    if (expr.empty()) {
        handleError(0, "empty expression");
    }
    for (size_t i = 0; i < expr.length(); ++i) {
        if (spaceFlag == true) {
            if (expr[i] == ' ') {
                if (i + 1 < expr.length()) {
                    spaceFlag = false;
                    continue;
                } else {
                    handleError(i + 1, "unexpected space");
                }
            } else {
                handleError(i + 1, "missing space");
            }
        } else if (std::isdigit(static_cast<unsigned char>(expr[i]))) {
            stack_.push(expr[i] - '0');
            spaceFlag = true;
        } else if (expr[i] == '+') {
            calc(ADD, i + 1);
            spaceFlag = true;
        } else if (expr[i] == '-') {
            calc(SUB, i + 1);
            spaceFlag = true;
        } else if (expr[i] == '*') {
            calc(MUL, i + 1);
            spaceFlag = true;
        } else if (expr[i] == '/') {
            calc(DIV, i + 1);
            spaceFlag = true;
        } else {
            handleError(i + 1, "unexpected character");
        }
    }
    if (!stack_.empty()) {
        ret = stack_.top();
        stack_.pop();
        if (!stack_.empty()) {
            handleError(0, "extra operand");
        }
    } else {
        throw std::runtime_error("unexpected error");
    }
    return ret;
}

void RPN::calc(CalcType type, size_t pos) {
    if (stack_.size() < 2) {
        handleError(pos, "insufficient operand");
    }
    int op2 = stack_.top();
    stack_.pop();
    int op1 = stack_.top();
    stack_.pop();
    if (type == ADD) {
        add(op1, op2, pos);
    } else if (type == SUB) {
        sub(op1, op2, pos);
    } else if (type == MUL) {
        mul(op1, op2, pos);
    } else {
        div(op1, op2, pos);
    }
}

void RPN::add(int op1, int op2, size_t pos) {
    if (op2 > 0 && op1 > std::numeric_limits<int>::max() - op2) {
        handleError(pos, "integer overflow");
    } else if (op2 < 0 && op1 < std::numeric_limits<int>::min() - op2) {
        handleError(pos, "integer underflow");
    }
    stack_.push(op1 + op2);
}

void RPN::sub(int op1, int op2, size_t pos) {
    if (op2 > 0 && op1 < std::numeric_limits<int>::min() + op2) {
        handleError(pos, "integer underflow");
    } else if (op2 < 0 && op1 > std::numeric_limits<int>::max() + op2) {
        handleError(pos, "integer overflow");
    }
    stack_.push(op1 - op2);
}

void RPN::mul(int op1, int op2, size_t pos) {
    if (op1 == std::numeric_limits<int>::min()) {
        if (op2 <= -1) {
            handleError(pos, "integer overflow");
        } else if (op2 > 1) {
            handleError(pos, "integer underflow");
        }
    } else if (op2 == std::numeric_limits<int>::min()) {
        if (op1 <= -1) {
            handleError(pos, "integer overflow");
        } else if (op1 > 1) {
            handleError(pos, "integer underflow");
        }
    } else {
        int abs1 = op1;
        if (op1 < 0)
            abs1 *= -1;
        int abs2 = op2;
        if (op2 < 0)
            abs2 *= -1;
        if (abs2 != 0 && abs1 > std::numeric_limits<int>::max() / abs2) {
            if ((op1 > 0) == (op2 > 0)) {
                handleError(pos, "integer overflow");
            } else {
                handleError(pos, "integer underflow");
            }
        }
    }
    stack_.push(op1 * op2);
}

void RPN::div(int op1, int op2, size_t pos) {
    if (op2 == 0) {
        handleError(pos, "division by zero");
    } else if (op1 == std::numeric_limits<int>::min() && op2 == -1) {
        handleError(pos, "integer overflow");
    }
    stack_.push(op1 / op2);
}

void RPN::handleError(size_t pos, const std::string& mes) {
    while (!stack_.empty()) {
        stack_.pop();
    }
    if (pos == 0) {
        throw std::runtime_error("Error: " + mes);
    } else {
        throw std::runtime_error("Error: " + tostr(pos) + ": " + mes);
    }
}
