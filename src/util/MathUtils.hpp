//
// Created by bananasmoothii on 20/08/2023.
//

#ifndef POWER4_MATH_UTILS_CPP
#define POWER4_MATH_UTILS_CPP

int intPow(int base, unsigned int exp) {
    int res = 1;
    while (exp) {
        if (exp & 1) // if exp is odd
            res *= base;
        exp >>= 1;
        base *= base; // base = base^2
    }
    return res;
}

#endif //POWER4_MATH_UTILS_CPP
