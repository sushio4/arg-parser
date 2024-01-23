# Argument Parser
## Introduction
Do you write cli programs? Are you tired of ctrl+c ctrl+v your argument parsing code all over your projects? Well, then, this library is for you!

It is a header-only library for automatic parsing of command line arguments

## Features
- support for boolean flags, named and unnamed arguments
- making some of the options compulsory through comulsory groups
- compile-time argument array
- O(n*m) parsing time complexity, where n is the number of declared arguments and m is the number of passed arguments

## Usage
### Setting up
Just `#include "ArgParser.hpp"` and you're ready to go.

### Initialization
Recommended way to do it is as follows:
- Use function `make_args()` to create a `constexpr std::array` of `arguments`
- Write some help message and version message
- Pass them to the constructor

Example:
```c++
#include "ArgParser"

constexpr auto args = ArgParser::make_args(
    ArgParser::Argument('h', "help", ArgParser::ArgType::help),
    //...
);

constexpr auto help = "...";
constexpr auto version = "...";

int main(int argc, char* argv[]){
    ArgParser::Parser p(args, help, version);
    p.parse(argc, argv);
    //...
}
```
You may want to use some `using` type aliases not to make it too verbose.

### Using the results
There are three functions that make it possible to use the results:
- `set()` checks how many times a regular flag was set, may serve as a bool
- `value()` returns the value of an appropriate exetended flag
- `unnamed_args()` returns a const reference to the vector of non-flag args

Example:

Suppose we call the program as such: `./a.out file1 file2 -o file3 -vv`

Here `file1` and `file2` are unnamed arguments so we obtain them by `p.unnamed_args()`

We want `-o` option to be followed by `file3` so `-o` is an extended argument and we obtain `file3` by calling `p.value('o')`

The `-v` flag is set twice, so a call to `p.set('v')` will return 2

### Compulsory groups
You can make groups of arguments in which one of the group must be provided by the user. You can specify it with `Argument` constructor

Example:
```c++
#include "ArgParser"

using Arg = ArgParser::Argument;
using ArgType = ArgParser::ArgType;

constexpr auto args = ArgParser::make_args(
    Arg('a', nullptr, ArgType::regular, 1),
    Arg('b', nullptr, ArgType::regular, 1),
    Arg('c', nullptr, ArgType::regular, 1),

    Arg('d', nullptr, ArgType::regular, 1),
    Arg('e', nullptr, ArgType::regular, 1)
    //...
);
```
In this case parser will return `false` if the user does not specify at least one of the flags a, b, c and one of the flags d, e.
