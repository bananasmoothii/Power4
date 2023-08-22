//
// Created by bananasmoothii on 21/08/2023.
//

#ifndef POWER4_OUTOFRANGEEXCEPTION_HPP
#define POWER4_OUTOFRANGEEXCEPTION_HPP

#include <string>
#include "TracedException.hpp"

class OutOfRangeException : public TracedException {
public:
    explicit OutOfRangeException(const std::string &message) : TracedException(message) {}
};


#endif //POWER4_OUTOFRANGEEXCEPTION_HPP
