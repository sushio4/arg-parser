#include "../ArgParser.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using ArgType = ArgParser::ArgType;
using Arg = ArgParser::Argument;

constexpr auto args = ArgParser::make_args(
    Arg('h', "help",   ArgType::help),
    Arg('V', "version",ArgType::version),
    //default is ArgType::regular
    Arg('v', "verbose"),
    //create first compulsory_group to enforce use of this option
    Arg('i', "infile", ArgType::extended, 1),
    //create second comulsory_group to also enforce this option
    Arg('o', "outfile",ArgType::extended, 2)
);

constexpr auto help_msg = 
    "Program for copying files\n"
    "Usage: ./example_1 -i (input file) -o (output file)\n\n"
    "All options:\n"
    " -i --infile   (input file) specifies file to copy from\n"
    " -o --outfile   (output file) specifies file to copy to\n"
    " -v --verbose  shows information\n"
    " -h --help     shows this message\n"
    " -V --version  shows version\n\n"
    "Written by Maciej Suski, 2024\n";
    
constexpr auto version_msg = 
    "Example 1: copying files v1.0\n"
    "Written by Maciej Suski, 2024\n";



int main(int argc, char* argv[]) {
    ArgParser::Parser p(args, help_msg, version_msg);

    if(!p.parse(argc, argv))
        return -1;

    std::ifstream ifile(p.value('i'));
    std::ofstream ofile(p.value('o'));

    if(!ifile || !ofile) {
        std::cerr << "Could not open a file!\n";
        return -1;
    }

    if(p.set('v')) {
        std::cout << "Opened files\n";
    }

    std::stringstream stream;
    stream << ifile.rdbuf();
    int count = stream.str().size();

    ofile << stream.str();

    if(p.set('v')) {
        std::cout << "Copied " << count << " characters from " << p.value('i') << " to " << p.value('o') << '\n';
    }

    ifile.close();
    ofile.close();
    
    if(p.set('v')) {
        std::cout << "Closed both files.\n";
    }
}