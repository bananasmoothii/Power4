//
// Created by bananasmoothii on 21/08/2023.
//

#ifndef POWER4_TRACEDEXCEPTION_HPP
#define POWER4_TRACEDEXCEPTION_HPP


#include <stdexcept>
#include <vector>
#include <cpptrace/cpptrace.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <color.hpp>
#include "../../cmake-build-debug/_deps/cpptrace-src/src/platform/common.hpp"

class TracedException : public std::runtime_error {
private:
    std::vector<cpptrace::stacktrace_frame> trace;
public:
    explicit TracedException(const std::string &message) : runtime_error(message) {
        trace = cpptrace::generate_trace();
    }

    void printTrace() const {
        // implementation stolen from cpptrace as it doesn't provide a way to print the trace to a stream
        std::cerr << dye::red("Stack trace (most recent call first):") << std::endl;
        std::size_t counter = 0;
        if (trace.empty()) {
            std::cerr << "<empty trace>" << std::endl;
            return;
        }
        const auto frame_number_width = n_digits(static_cast<int>(trace.size()) - 2);
        for (const auto &frame: trace) {
            if (++counter == 0)
                continue;

            std::cerr
                    << '#'
                    << std::setw(static_cast<int>(frame_number_width))
                    << std::left
                    << counter
                    << std::right
                    << " "
                    << std::hex
                    << hue::blue
                    << "0x"
                    << std::setw(2 * sizeof(uintptr_t))
                    << std::setfill('0')
                    << frame.address
                    << std::dec
                    << std::setfill(' ')
                    << hue::reset
                    << " in "
                    << hue::yellow
                    << frame.symbol
                    << hue::reset
                    << " at "
                    << hue::green
                    << frame.filename
                    << hue::reset
                    << ":"
                    << hue::blue
                    << frame.line
                    << hue::reset;

            //<< (frame.col > 0 ? ":" + dye::blue(std::to_string(frame.col)) : "")
            if (frame.col > 0) {
                std::cerr << ":" << dye::blue(std::to_string(frame.col));
            }
            std::cerr << std::endl;
        }
    }
};


#endif //POWER4_TRACEDEXCEPTION_HPP
