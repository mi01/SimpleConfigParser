#include <cstdlib>
#include <iostream>

#include "ConfigParser.h"

int main (int argc, char *argv[])
{
    // You could also change the delim and comment character here
    ConfigParser config("config.cfg");

    int foo = config.getInt("foo").second;
    std::string bar = config.getString("bar").second;
    std::pair<bool, double> pi = config.getDouble("pi");

    // If optional flag is set it will produce no error
    auto notExisting = config.getString("notExisting", true);

    // Check if key was found in config file
    if (!notExisting.first)
    {
        std::cerr << "Optional key not found in config!" << std::endl;
    }

    // You can also use a fallback value in case of an not existing key
    std::string fallback = config.getString("notExisting", std::string("fallback")).second;

    if(config.hasErrors())
    {
        std::cerr << "ERROR: Configuration incomplete!" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << bar << std::endl;
    std::cout << foo << std::endl;
    std::cout << pi.second << std::endl;
    std::cout << fallback << std::endl;

    return EXIT_SUCCESS;
} 