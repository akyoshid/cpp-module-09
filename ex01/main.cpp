/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyoshid <akyoshid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:17:41 by akyoshid          #+#    #+#             */
/*   Updated: 2025/09/12 22:30:36 by akyoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stdexcept>
#include "RPN.hpp"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./RPN [expression ...]" << std::endl;
        return 1;
    }
    RPN rpn;
    int ret = 0;
    for (int i = 1; i < argc; ++i) {
        try {
            std::cout << rpn.exec(argv[i]) << std::endl;
            ret = 0;
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
            ret = 1;
        }
    }
    return ret;
}
