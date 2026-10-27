#include <boost/version.hpp>
#include <iostream>

int main() {
    std::cout << "Hello, Boost!\n";
    std::cout << "Boost version: "
              << BOOST_LIB_VERSION << '\n';

    return 0;
}