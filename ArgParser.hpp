#pragma once
#include <array>
#include <vector>
#include <cstring>
#include <ostream>

namespace ArgParser {
    enum class Type : unsigned char {
        regular,
        extended,
        help,
        version
    };

    struct Argument {
        char flag = '\0';
        const char* full_flag = nullptr;
        Type type = Type::regular;
        int compulsory_group = 0;

        constexpr inline Argument(char flag, const char* full_flag, Type type = Type::regular, int compulsory_group = 0) 
        : flag(flag), full_flag(full_flag), type(type), compulsory_group(compulsory_group) {}

        constexpr inline Argument(char flag, const char* full_flag, int compulsory_group) 
        : flag(flag), full_flag(full_flag), compulsory_group(compulsory_group) {}  


        friend inline constexpr bool operator==(const Argument& lhs, const char* rhs) {
            if(lhs.full_flag == nullptr || rhs == nullptr || rhs[0] == '\0') return false;
            //long form
            return strcmp(lhs.full_flag, rhs) == 0;
        }

        friend inline constexpr bool operator==(const Argument& lhs, char rhs) {
            return lhs.flag == rhs;
        }
    };

    template<typename T, typename... Ts>
    struct array_type {
        using type = T;
    };

    template<typename... Ts>
    constexpr auto make_args(const Ts&&... values) {
        using array_type = typename array_type<Ts...>::type;
        return std::array<array_type, sizeof...(Ts)>{values...};
    }

    template<typename Type, std::size_t Size>
    //template<typename Array>
    class Parser {
    public:
        /**
         * @param args array of ArgParser::Argument structs. You can make it via make_args function 
        */
        constexpr Parser(const std::array<Type, Size>& args, const char* help_msg, const char* version_msg) : 
            _args{args}, _values{0}, _comp_groups(0), _help(help_msg), _version(version_msg) {
            //find max comp_group number, help and version args
            for(auto a : _args) {
                if(a.compulsory_group > _comp_groups) 
                    _comp_groups = a.compulsory_group;

                if(a.type == ArgParser::Type::help)
                    _help_ref = &a;
        
                if(a.type == ArgParser::Type::version)
                    _version_ref = &a;
            }
            _comp_groups++;
        }

        /**
         * @return returns true on success, false otherwise
        */
        bool parse(int argc, char* argv[]);

        /**
         * @return returns how many times the flag was set
        */
        template<typename AType>
        int set(AType arg) {
            for(std::size_t i = 0; i < Size; i++) {
                if(!(_args[i] == arg)) 
                    continue;

                if(_args[i].type == ArgParser::Type::extended) 
                    return 0;

                return _values[i].set;
            }
            return 0;
        }

        /**
         * @return returns string after an extended flag (--flag string) or nullptr
        */
        template<typename AType>
        const char* value(AType arg) {
            for(std::size_t i = 0; i < Size; i++) {
                if(!(_args[i] == arg)) 
                    continue;

                if(_args[i].type != Type::extended) 
                    return nullptr;

                return _values[i].str;
            }
            return nullptr;
        }

    private:
        /**
         * @brief searches for matching argument
         * @return index in array or -1 on failure
        */
        int search_args(const char* arg);
        /**
         * @brief searches for matching argument
         * @return index in array or -1 on failure
        */
        int search_args(char arg);

        void show_help(std::ostream& out);
        void show_version();
        void print_comp_group(int group);

        void show_special(ArgParser::Type type);

        union ArgValue{
            int   set = 0;
            char* str;
        };

        std::array<Type, Size>      _args;
        std::array<ArgValue, Size>  _values;
        std::vector<char*>          _unnamed;

        Argument* _help_ref = nullptr;
        Argument* _version_ref = nullptr;
        
        int _comp_groups;

        const char* _help;
        const char* _version;
    };

}

#include "ArgParser_impl.hpp"
