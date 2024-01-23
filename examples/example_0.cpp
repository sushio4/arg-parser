#include "../ArgParser.hpp"
#include <iostream>

using ArgType = ArgParser::ArgType;
using Arg = ArgParser::Argument;

constexpr auto args = ArgParser::make_args(
    Arg('h', "help",   ArgType::help),
    Arg('V', "version",ArgType::version)
);

constexpr auto help_msg = "Help message\n";
constexpr auto version_msg = "Version\n";

int main(int argc, char* argv[]) {
    ArgParser::Parser p(args, help_msg, version_msg);

    if(!p.parse(argc, argv))
        return -1;
}