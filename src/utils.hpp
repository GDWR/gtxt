#ifndef GTXT_UTILS_HPP
#define GTXT_UTILS_HPP

#include <string>
#include <algorithm>

std::string to_string_repr(int integer) {
    std::string output;

    while (true) {
        int num = integer % 10;
        integer = integer / 10;

        output += '1' + (num - 1);

        if (integer == 0) {
            break;
        }
    }

    std::reverse(output.begin(), output.end());
    return output;
}

#endif //GTXT_UTILS_HPP
