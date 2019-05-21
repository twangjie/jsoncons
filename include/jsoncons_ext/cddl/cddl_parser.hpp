// Copyright 2019 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_CDDL_CDDL_PARSER_HPP
#define JSONCONS_CDDL_CDDL_PARSER_HPP

#include <jsoncons/json.hpp>
#include <jsoncons_ext/cddl/cddl_error.hpp>

namespace jsoncons { namespace cddl {

template <class CharT>
class basic_cddl_specification
{
public:
    typedef CharT char_type;

    static basic_cddl_specification parse(const std::basic_string<CharT>& s);
};

enum class cddl_state : uint8_t
{
    expect_rule,
    id,
    memberkey,
    expect_assign,
    expect_colon_or_right_bracket,
    expect_groupent,
    expect_value,
    expect_memberkey,
    value,
    minus,
    digit1,
    hex_number_value,
    zero_digit,
    fraction,
    plus_minus_exponent,
    exponent,
    quoted_value,
    expect_rangeop_or_slash_or_comma_or_right_bracket,
    expect_rangeop,
    expect_exclusive_or_inclusive_rangeop,
    array_definition,
    array_definition2,
    map_definition,
    map_definition2,
    group,
    group2,
    after_value,
    foo
};

template <class CharT>
class basic_cddl_parser
{
public:
    typedef CharT char_type;
private:
    const char_type* p_;
    const char_type* endp_; 
    size_t line_;
    size_t column_;

    struct state_item
    {
        cddl_state state;
        char_type delimiter;

        explicit state_item(cddl_state state)
            : state(state), delimiter(0)
        {
        }

        state_item(cddl_state state, char_type delimiter)
            : state(state), delimiter(delimiter)
        {
        }

        state_item(cddl_state state, const state_item& parent)
            : state(state), delimiter(parent.delimiter)
        {
        }
    };
public:
    basic_cddl_parser()
        : line_(1), column_(1)
    {
    }
    basic_cddl_specification<char_type> parse(const std::basic_string<char_type>& s)
    {
        std::vector<state_item> state_stack;

        state_stack.emplace_back(cddl_state::expect_rule);

        std::basic_string<char_type> buffer;

        p_ = s.data();
        endp_ = s.data() + s.length(); 

        while (p_ < endp_)
        {
            switch (state_stack.back().state)
            {
                case cddl_state::expect_rule: 
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            advance_past_space_character();
                            break;
                        case ';':
                            skip_to_end_of_line();
                            break;
                        default:
                            if (is_ealpha(*p_))
                            {
                                buffer.clear();
                                buffer.push_back(*p_);
                                state_stack.emplace_back(cddl_state::foo);
                                state_stack.emplace_back(cddl_state::id);
                                ++p_;
                                ++column_;
                            }
                            else
                            {
                                throw ser_error(cddl_errc::expected_id,line_,column_);
                            }
                            break;
                    }
                    break;
                }
                case cddl_state::foo:
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            advance_past_space_character();
                            break;
                        case ';':
                            skip_to_end_of_line();
                            break;
                        default:
                            state_stack.back().state = cddl_state::expect_assign;
                            break;
                    }
                    break;
                }
                case cddl_state::expect_assign: 
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            advance_past_space_character();
                            break;
                        case ';':
                            skip_to_end_of_line();
                            break;
                        case '=':
                            state_stack.back().state = cddl_state::expect_groupent;
                            ++p_;
                            ++column_;
                            break;
                        default:
                            throw ser_error(cddl_errc::expected_assign,line_,column_);
                            break;
                    }
                    break;
                }
                case cddl_state::expect_value: 
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            advance_past_space_character();
                            break;
                        case ';':
                            skip_to_end_of_line();
                            break;
                        case '\"':
                            buffer.clear();
                            state_stack.back().state = cddl_state::quoted_value;
                            ++p_;
                            ++column_;
                            break;
                        case '-':
                            buffer.clear();
                            buffer.push_back(*p_);
                            state_stack.back().state = cddl_state::minus;
                            ++p_;
                            ++column_;
                            break;
                        case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8': case '9':
                            buffer.clear();
                            buffer.push_back(*p_);
                            state_stack.back().state = cddl_state::digit1;
                            ++p_;
                            ++column_;
                            break;
                        case '0': 
                            buffer.clear();
                            buffer.push_back(*p_);
                            state_stack.back().state = cddl_state::zero_digit;
                            ++p_;
                            ++column_;
                            break;
                        default:
                            buffer.clear();
                            state_stack.back().state = cddl_state::value;
                            break;
                    }
                    break;
                }
                case cddl_state::minus: 
                {
                    switch (*p_)
                    {
                        case '0': 
                            buffer.clear();
                            buffer.push_back(*p_);
                            state_stack.back().state = cddl_state::zero_digit;
                            ++p_;
                            ++column_;
                            break;
                        case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8': case '9':
                            buffer.clear();
                            buffer.push_back(*p_);
                            state_stack.back().state = cddl_state::digit1;
                            ++p_;
                            ++column_;
                            break;
                        default:
                            throw ser_error(cddl_errc::invalid_number,line_,column_);
                            break;
                    }
                    break;
                }
                case cddl_state::expect_memberkey:
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            advance_past_space_character();
                            break;
                        case ';':
                            skip_to_end_of_line();
                            break;
                        default:
                            if (*p_ == state_stack.back().delimiter)
                            {
                                state_stack.pop_back();
                            }
                            else
                            {
                                buffer.clear();
                                state_stack.back().state = cddl_state::memberkey;
                            }
                            break;
                    }
                    break;
                }
                case cddl_state::expect_colon_or_right_bracket:
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            advance_past_space_character();
                            break;
                        case ';':
                            skip_to_end_of_line();
                            break;
                        case ':':
                            state_stack.back().state = cddl_state::expect_rangeop_or_slash_or_comma_or_right_bracket;
                            state_stack.emplace_back(cddl_state::expect_value, state_stack.back());
                            ++p_;
                            ++column_;
                            break;
                        default:
                            if (*p_ == state_stack.back().delimiter)
                            {
                                state_stack.pop_back();
                            }
                            else
                            {
                                throw ser_error(cddl_errc::expected_assign,line_,column_);
                            }
                            break;
                    }
                    break;
                }
                case cddl_state::array_definition: 
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            advance_past_space_character();
                            break;
                        case ';':
                            skip_to_end_of_line();
                            break;
                        case ']':
                            ++p_;
                            ++column_;
                            state_stack.pop_back();
                            break;
                        case '(':
                            ++p_;
                            ++column_;
                            state_stack.emplace_back(cddl_state::group, ')');
                            break;
                        default:
                            buffer.clear();
                            buffer.push_back(*p_);
                            state_stack.back().state = cddl_state::array_definition2;
                            state_stack.emplace_back(cddl_state::memberkey, ']');
                            ++p_;
                            ++column_;
                            break;
                    }
                    break;
                }
                case cddl_state::array_definition2: 
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            advance_past_space_character();
                            break;
                        case ';':
                            skip_to_end_of_line();
                            break;
                        case ']':
                            state_stack.pop_back();
                            ++p_;
                            ++column_;
                            break;
                        case ',':
                            buffer.clear();
                            state_stack.emplace_back(cddl_state::expect_memberkey,']');
                            ++p_;
                            ++column_;
                            break;
                        case '(':
                            ++p_;
                            ++column_;
                            state_stack.emplace_back(cddl_state::group, ')');
                            break;
                        default:
                            throw ser_error(cddl_errc::expected_comma_or_left_par_or_right_sqbracket,line_,column_);
                            break;
                    }
                    break;
                }
                case cddl_state::map_definition: 
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            advance_past_space_character();
                            break;
                        case ';':
                            skip_to_end_of_line();
                            break;
                        case '}':
                            ++p_;
                            ++column_;
                            state_stack.pop_back();
                            break;
                        case '(':
                            ++p_;
                            ++column_;
                            state_stack.emplace_back(cddl_state::group, ')');
                            break;
                        default:
                            buffer.clear();
                            buffer.push_back(*p_);
                            state_stack.back().state = cddl_state::map_definition2;
                            state_stack.emplace_back(cddl_state::memberkey, '}');
                            ++p_;
                            ++column_;
                            break;
                    }
                    break;
                }
                case cddl_state::map_definition2: 
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            advance_past_space_character();
                            break;
                        case ';':
                            skip_to_end_of_line();
                            break;
                        case ',':
                            buffer.clear();
                            state_stack.emplace_back(cddl_state::expect_memberkey, '}');
                            ++p_;
                            ++column_;
                            break;
                        case '}':
                            ++p_;
                            ++column_;
                            state_stack.pop_back();
                            break;
                        case '(':
                            ++p_;
                            ++column_;
                            state_stack.emplace_back(cddl_state::group, ')');
                            break;
                        default:
                            throw ser_error(cddl_errc::expected_comma_or_left_par_or_right_curbracket,line_,column_);
                            break;
                    }
                    break;
                }
                case cddl_state::group: 
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            advance_past_space_character();
                            break;
                        case ';':
                            skip_to_end_of_line();
                            break;
                        case ')':
                            ++p_;
                            ++column_;
                            state_stack.pop_back();
                            break;
                        default:
                            buffer.clear();
                            buffer.push_back(*p_);
                            state_stack.back().state = cddl_state::group2;
                            state_stack.emplace_back(cddl_state::memberkey, ')');
                            ++p_;
                            ++column_;
                            break;
                    }
                    break;
                }
                case cddl_state::group2: 
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            advance_past_space_character();
                            break;
                        case ';':
                            skip_to_end_of_line();
                            break;
                        case ',':
                            buffer.clear();
                            state_stack.emplace_back(cddl_state::expect_memberkey, ')');
                            ++p_;
                            ++column_;
                            break;
                        case ')':
                            ++p_;
                            ++column_;
                            state_stack.pop_back();
                            break;
                        default:
                            throw ser_error(cddl_errc::expected_comma_or_right_par,line_,column_);
                            break;
                    }
                    break;
                }
                case cddl_state::expect_groupent: 
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            advance_past_space_character();
                            break;
                        case ';':
                            skip_to_end_of_line();
                            break;
                        case '[':
                            ++p_;
                            ++column_;
                            state_stack.back().state = cddl_state::array_definition;
                            break;
                        case '{':
                            ++p_;
                            ++column_;
                            state_stack.back().state = cddl_state::map_definition;
                            break;
                        case '(':
                            ++p_;
                            ++column_;
                            state_stack.back().state = cddl_state::group;
                            break;
                        default:
                            buffer.clear();
                            state_stack.back().state = cddl_state::expect_rangeop_or_slash_or_comma_or_right_bracket;
                            state_stack.emplace_back(cddl_state::expect_value, state_stack.back());
                            break;
                    }
                    break;
                }

                case cddl_state::id: 
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':case '=':
                            if (is_hyphen_or_dot(buffer.back()))
                            {
                                throw ser_error(cddl_errc::invalid_id,line_,column_);
                            }
                            state_stack.pop_back();
                            std::cout << "id: " << buffer << "\n";
                            break;
                        default:
                            if (is_ealpha(*p_) || is_digit(*p_) || is_hyphen_or_dot(*p_))
                            {
                                buffer.push_back(*p_);
                                ++p_;
                                ++column_;
                            }
                            else
                            {
                                throw ser_error(cddl_errc::invalid_id,line_,column_);
                            }
                            break;
                    }
                    break;
                }
                case cddl_state::memberkey: 
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            std::cout << "memberkey: " << buffer << "\n";
                            advance_past_space_character();
                            state_stack.back().state = cddl_state::expect_colon_or_right_bracket;
                            break;
                        case ';':
                            skip_to_end_of_line();
                            break;
                        case ':':
                            std::cout << "memberkey: " << buffer << "\n";
                            state_stack.back().state = cddl_state::expect_colon_or_right_bracket;
                            break;
                        default:
                            if (*p_ == state_stack.back().delimiter)
                            {
                                std::cout << "reference: " << buffer << "\n";
                                state_stack.pop_back();
                            }
                            else
                            {
                                buffer.push_back(*p_);
                                ++p_;
                                ++column_;
                            }
                            break;
                    }
                    break;
                }
                case cddl_state::value: 
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            std::cout << "value: " << buffer << "\n";
                            advance_past_space_character();
                            state_stack.pop_back();
                            break;
                        case ',':
                            std::cout << "value: " << buffer << "\n";
                            state_stack.pop_back();
                            break;
                        default:
                            if (*p_ == state_stack.back().delimiter)
                            {
                                std::cout << "value: " << buffer << "\n";
                                state_stack.pop_back();
                            }
                            else
                            {
                                buffer.push_back(*p_);
                                ++p_;
                                ++column_;
                            }
                            break;
                    }
                    break;
                }
                case cddl_state::digit1: 
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            std::cout << "number: " << buffer << "\n";
                            advance_past_space_character();
                            state_stack.pop_back();
                            break;
                        case ',':
                            std::cout << "number: " << buffer << "\n";
                            state_stack.pop_back();
                            break;
                        case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8': case '9':
                            buffer.push_back(*p_);
                            ++p_;
                            ++column_;
                            break;
                        case '.':
                            state_stack.back().state = cddl_state::fraction;
                            buffer.push_back(*p_);
                            ++p_;
                            ++column_;
                            break;
                        case 'e':
                            state_stack.back().state = cddl_state::plus_minus_exponent;
                            buffer.push_back(*p_);
                            ++p_;
                            ++column_;
                            break;
                        default:
                            if (*p_ == state_stack.back().delimiter)
                            {
                                std::cout << "value: " << buffer << "\n";
                                state_stack.pop_back();
                            }
                            else
                            {
                                throw ser_error(cddl_errc::invalid_id,line_,column_);
                            }
                            break;
                    }
                    break;
                }
                case cddl_state::plus_minus_exponent: 
                {
                    switch (*p_)
                    {
                        case '+':
                        case '-':
                        case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8': case '9':
                            buffer.push_back(*p_);
                            ++p_;
                            ++column_;
                            state_stack.back().state = cddl_state::exponent;
                            break;
                        default:
                            if (*p_ == state_stack.back().delimiter)
                            {
                                std::cout << "value: " << buffer << "\n";
                                state_stack.pop_back();
                            }
                            else
                            {
                                throw ser_error(cddl_errc::invalid_id,line_,column_);
                            }
                            break;
                    }
                    break;
                }
                case cddl_state::exponent: 
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            std::cout << "number: " << buffer << "\n";
                            advance_past_space_character();
                            state_stack.pop_back();
                            break;
                        case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8': case '9':
                            buffer.push_back(*p_);
                            ++p_;
                            ++column_;
                            break;
                        default:
                            if (*p_ == state_stack.back().delimiter)
                            {
                                std::cout << "value: " << buffer << "\n";
                                state_stack.pop_back();
                            }
                            else
                            {
                                throw ser_error(cddl_errc::invalid_id,line_,column_);
                            }
                            break;
                    }
                    break;
                }
                case cddl_state::fraction: 
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            std::cout << "number: " << buffer << "\n";
                            advance_past_space_character();
                            state_stack.pop_back();
                            break;
                        case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8': case '9':
                            buffer.push_back(*p_);
                            ++p_;
                            ++column_;
                            break;
                        case 'e':
                            state_stack.back().state = cddl_state::plus_minus_exponent;
                            buffer.push_back(*p_);
                            ++p_;
                            ++column_;
                            break;
                        default:
                            if (*p_ == state_stack.back().delimiter)
                            {
                                std::cout << "value: " << buffer << "\n";
                                state_stack.pop_back();
                            }
                            else
                            {
                                throw ser_error(cddl_errc::invalid_id,line_,column_);
                            }
                            break;
                    }
                    break;
                }
                case cddl_state::hex_number_value: 
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            std::cout << "number: " << buffer << "\n";
                            advance_past_space_character();
                            state_stack.pop_back();
                            break;
                        case ',':
                            std::cout << "number: " << buffer << "\n";
                            state_stack.pop_back();
                            break;
                        default:
                            if (*p_ == state_stack.back().delimiter)
                            {
                                std::cout << "value: " << buffer << "\n";
                                state_stack.pop_back();
                            }
                            else
                            {
                                buffer.push_back(*p_);
                                ++p_;
                                ++column_;
                            }
                            break;
                    }
                    break;
                }
                case cddl_state::zero_digit: 
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            std::cout << "number: " << buffer << "\n";
                            advance_past_space_character();
                            state_stack.pop_back();
                            break;
                        case ',':
                            std::cout << "number: " << buffer << "\n";
                            state_stack.pop_back();
                            break;
                        case 'x':
                            buffer.push_back(*p_);
                            state_stack.back().state = cddl_state::hex_number_value;
                            ++p_;
                            ++column_;
                            break;
                        default:
                            buffer.push_back(*p_);
                            state_stack.back().state = cddl_state::digit1;
                            ++p_;
                            ++column_;
                            break;
                    }
                    break;
                }
                case cddl_state::expect_rangeop_or_slash_or_comma_or_right_bracket: 
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            advance_past_space_character();
                            break;
                        case ',':
                            state_stack.pop_back();
                            break;
                        case '/':
                            state_stack.emplace_back(cddl_state::expect_value, state_stack.back());
                            ++p_;
                            ++column_;
                            break;
                        case '.':
                            state_stack.back().state = cddl_state::expect_rangeop;
                            ++p_;
                            ++column_;
                            break;
                        default:
                            switch (state_stack.back().delimiter)
                            {
                                case 0:
                                    state_stack.pop_back();
                                    break;
                                default:
                                    if (*p_ == state_stack.back().delimiter)
                                    {
                                        state_stack.pop_back();
                                    }
                                    else
                                    {
                                        throw ser_error(cddl_errc::expected_rangeop_or_slash_or_comma_or_right_bracket,line_,column_);
                                    }
                                    break;
                            }
                            break;
                    }
                    break;
                }
                case cddl_state::expect_rangeop: 
                {
                    switch (*p_)
                    {
                        case '.':
                            state_stack.back().state = cddl_state::expect_exclusive_or_inclusive_rangeop;
                            ++p_;
                            ++column_;
                            break;
                        default:
                            throw ser_error(cddl_errc::expected_rangeop_or_slash_or_comma_or_right_bracket,line_,column_);
                            break;
                    }
                    break;
                }
                case cddl_state::expect_exclusive_or_inclusive_rangeop: 
                {
                    switch (*p_)
                    {
                        case '.': // inclusive
                            state_stack.back().state = cddl_state::expect_rangeop_or_slash_or_comma_or_right_bracket;
                            state_stack.emplace_back(cddl_state::expect_value, state_stack.back());
                            ++p_;
                            ++column_;
                            break;
                        default:
                            state_stack.back().state = cddl_state::expect_rangeop_or_slash_or_comma_or_right_bracket;
                            state_stack.emplace_back(cddl_state::expect_value, state_stack.back());
                            break;
                    }
                    break;
                }
                case cddl_state::quoted_value: 
                {
                    switch (*p_)
                    {
                        case '\"':
                            std::cout << "value: " << buffer << "\n";
                            state_stack.pop_back();
                            ++p_;
                            ++column_;
                            break;
                        default:
                            buffer.push_back(*p_);
                            ++p_;
                            ++column_;
                            break;
                    }
                    break;
                }
                default:
                {
                    JSONCONS_UNREACHABLE();
                }
            }
        }

        std::cout << "stack size: " << state_stack.size() << "\n";
        for (auto item : state_stack)
        {
            std::cout << (int)item.state << "\n";
        }

        return basic_cddl_specification<CharT>();
    }
private:

    void advance_past_space_character()
    {
        switch (*p_)
        {
            case ' ':case '\t':
                ++p_;
                ++column_;
                break;
            case '\r':
                if (p_+1 < endp_ && *(p_+1) == '\n')
                    ++p_;
                ++line_;
                column_ = 1;
                ++p_;
                break;
            case '\n':
                ++line_;
                column_ = 1;
                ++p_;
                break;
            default:
                break;
        }
    }

    bool is_hyphen_or_dot(char_type c)
    {
        return c == '-' || c == '.';
    }

    bool is_digit(char_type c)
    {
        return (c >= 0x30 && c <= 0x39);
    }

    bool is_alpha(char_type c)
    {
        return (c >= 0x41 && c <= 0x5A) || (c >= 0x61 && c <= 0x7A);
    }

    bool is_ealpha(char_type c)
    {
        switch (c)
        {
            case '@':
            case '_':
            case '$':
                return true;
            default:
                return is_alpha(c);
        }
    }

    void skip_to_end_of_line()
    {
        while (p_ < endp_)
        {
            switch (*p_)
            {
                case '\r':
                case '\n':
                    advance_past_space_character();
                    return;
                default:
                    ++p_;
                    ++column_;
                    break;
            }
        }
    }
};

template <class CharT>
basic_cddl_specification<CharT> basic_cddl_specification<CharT>::parse(const std::basic_string<CharT>& s)
{
    basic_cddl_parser<CharT> parser;
    return parser.parse(s);
}

typedef basic_cddl_specification<char> cddl_specification;

}}

#endif
