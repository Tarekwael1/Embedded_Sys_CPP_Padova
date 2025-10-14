#ifndef helper_functions_h
#define helper_functions_h

#include <vector>


namespace PST {

    std::vector<int>::const_iterator find_first(const std::vector<int>& a, int value);
    void remove_even_elements(std::vector<int> &v);
}





#endif /* helper_functions_h */
