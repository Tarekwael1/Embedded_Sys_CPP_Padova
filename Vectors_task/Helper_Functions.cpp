#include "Helper_Functions.h"

namespace PST {

std::vector<int>::const_iterator find_first(const std::vector<int>& a, int value)
{
    for (auto it = a.begin(); it != a.end(); ++it) {
        if (*it == value) {
            return it;
        }
    }
    return a.end();
}


void remove_even_elements(std::vector<int> &v)
{
    for (auto it = v.begin(); it != v.end(); )
    {
        if (*it % 2 == 0) {
            it = v.erase(it);
        } else {
            ++it;
        }
    }
}

}
