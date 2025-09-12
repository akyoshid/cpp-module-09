/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyoshid <akyoshid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 16:41:11 by akyoshid          #+#    #+#             */
/*   Updated: 2025/09/12 23:46:31 by akyoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EX01_UTILS_HPP_
# define EX01_UTILS_HPP_

# include <string>
# include <sstream>

template <typename T>
std::string tostr(T num) {
    std::ostringstream oss;
    oss << num;
    return oss.str();
}

#endif
