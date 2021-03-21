//#define USE_BUILD_MAIN

#ifdef USE_BUILD_MAIN

#include <iostream>

int main()
{
    std::cout << "It has been ran." << std::endl;
}


#else
#define CATCH_CONFIG_MAIN
#include "thirdPartyLibraries/catch2/catch.hpp"
#endif