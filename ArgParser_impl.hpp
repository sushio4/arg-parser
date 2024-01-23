#pragma once
#include "ArgParser.hpp"
#include <iostream>

namespace ArgParser {

    template<typename Type, std::size_t Size>
    int Parser<Type, Size>::search_args(char arg) {
        for(std::size_t i = 0; i < Size; i++) {
            if(_args[i] == arg) return i;
        }
        std::cerr << "Unrecognized option: -" << arg << "\n\n";

        show_help(std::cerr);

        return -1;
    }   

    template<typename Type, std::size_t Size>
    int Parser<Type, Size>::search_args(const char* arg) {
        for(std::size_t i = 0; i < Size; i++) {
            if(_args[i] == arg) return i;
        }
        std::cerr << "Unrecognized option: --" << arg << "\n\n";

        show_help(std::cerr);
        
        return -1;
    }   

    template<typename Type, std::size_t Size>
    void Parser<Type, Size>::show_help(std::ostream& out) {
        if(_help == nullptr) return;
        out << _help;
    }

    template<typename Type, std::size_t Size>
    void Parser<Type, Size>::show_version() {
        if(_version == nullptr) return;
        std::cout << _version;
    }

    template<typename Type, std::size_t Size>
    void Parser<Type, Size>::show_special(ArgParser::Type type) {
        if(type == ArgParser::Type::help)
            show_help(std::cout);
        else if(type == ArgParser::Type::version)
            show_version();
    }

    template<typename Type, std::size_t Size>
    void Parser<Type, Size>::print_comp_group(int group) {

        bool first = true;

        for(std::size_t i = 0; i < Size; i++) {
            if(_args[i].compulsory_group != group)
                continue;

            if(first) {
                std::cerr << '-' << _args[i].flag << " |--" << _args[i].full_flag;
                first = false;
                continue;
            }
            std::cerr << ",  -" << _args[i].flag << " |--" << _args[i].full_flag;
        }
        std::cerr << "\n\n";
        show_help(std::cerr);
    }


    template<typename Type, std::size_t Size>
    bool Parser<Type, Size>::parse(int argc, char* argv[]) {
        /**array for checking compulsory groups
         * since groups start from 1, the 0 value tells whether to ignore the rest 
         */
        bool comp_satisfied[_comp_groups]{0};

        for(int i = 1; i < argc; i++) {
            //check for unnamed args
            if(argv[i][0] != '-') {
                _unnamed.push_back(argv[i]);
                continue;
            }

            auto parse_arg = [&](int index) {
                using ArgType = ArgParser::Type;

                if(_args[index].type == ArgType::extended) {
                    i++;
                    _values[index].str = argv[i];
                    return;
                }

                _values[index].set++;
                
                //special treatment for help/version
                if(_args[index].type != ArgType::regular) {
                    comp_satisfied[0] = true;
                    
                    show_special(_args[index].type);

                    return;
                }

                //compulsory group check
                if(_args[index].compulsory_group != 0) {
                    comp_satisfied[_args[index].compulsory_group] = true;
                }
            };

            //check for long
            if(argv[i][1] == '-') {
                int index = search_args(argv[i] + 2);
                if(index == -1) return false;
                
                parse_arg(index);
                continue;
            } 

            //short
            for(auto p = argv[i] + 1; *p; p++) {
                int index = search_args(*p);
                if(index == -1) return false;

                parse_arg(index);
            }
        }

        //comp check
        if(comp_satisfied[0]) return true;

        for(int i = 1; i < _comp_groups; i++) {
            if(comp_satisfied[i]) 
                continue;

            std::cerr << "At least one of the following arguments has to be included:\n";
            print_comp_group(i);
            
            return false;      
        }
        return true;
    }

}