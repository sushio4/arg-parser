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