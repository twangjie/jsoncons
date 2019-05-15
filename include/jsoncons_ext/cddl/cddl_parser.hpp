// Copyright 2019 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_CDDL_CDDL_PARSER_HPP
#define JSONCONS_CDDL_CDDL_PARSER_HPP

#include <jsoncons/json.hpp>

namespace jsoncons { namespace cddl {

template <class CharT>
class basic_cddl_specification
{
public:
    typedef CharT char_type;

    static basic_cddl_specification parse(const std::basic_string<CharT>& s);
};

enum class cddl_state 
{
    start,
    name,
    expect_equal,
    expect_group,
    group
};

struct state_item
{
    cddl_state state;

    state_item(cddl_state state)
        : state(state)
    {
    }
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
public:
    basic_cddl_specification<char_type> parse(const std::basic_string<char_type>& s)
    {
        std::vector<state_item> state_stack;

        state_stack.emplace_back(cddl_state::start);

        std::basic_string<char_type> buffer;

        p_ = s.data();
        endp_ = s.data() + s.length(); 

        while (p_ < endp_)
        {
            switch (state_stack.back().state)
            {
                case cddl_state::start: 
                {
                    switch (*p_)
                    {
                        default:
                            buffer.push_back(*p_);
                            state_stack.back() = cddl_state::name;
                            ++p_;
                            break;
                    }
                    break;
                }
                case cddl_state::expect_equal: 
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            advance_past_space_character();
                            break;
                        case '=':
                            state_stack.back() = cddl_state::expect_group;
                            ++p_;
                            break;
                        default:
                            throw std::runtime_error("Expected =");
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
                        default:
                            ++p_;
                            break;
                    }
                    break;
                }
                case cddl_state::expect_group: 
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            advance_past_space_character();
                            break;
                        case '{':
                            ++p_;
                            state_stack.back() = cddl_state::group;
                            break;
                        case '(':
                            ++p_;
                            state_stack.back() = cddl_state::group;
                            break;
                        default:
                            throw std::runtime_error("Expected { or (");
                            break;
                    }
                    break;
                }
                case cddl_state::name: 
                {
                    switch (*p_)
                    {
                        case ' ': case '\t': case '\r': case '\n':
                            advance_past_space_character();
                            state_stack.back() = cddl_state::expect_equal;
                            break;
                        default:
                            buffer.push_back(*p_);
                            ++p_;
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
