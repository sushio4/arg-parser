/*  Copyright (C) 2024  Maciej Suski  suskimaciej@interia.pl

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../ArgParser.hpp"
#include <iostream>

constexpr auto args = ArgParser::make_args(
    ArgParser::Argument('h', "help", ArgParser::ArgType::help),
    ArgParser::Argument('v', "version", ArgParser::ArgType::version)
);

constexpr auto help = 
    "Usage ./example_2 args...\n\n"
    "By Maciej Suski, 2024\n";

constexpr auto version = 
    "Example no.2 v1\n\n"
    "by Maciej Suski, 2024\n";

int main(int argc, char* argv[]) {
    ArgParser::Parser p(args, help, version);

    if(!p.parse(argc, argv)) 
        return -1;
    
    auto args = p.unnamed_args();

    for(auto a : args) {
        std::cout << a << '\n';
    }
}