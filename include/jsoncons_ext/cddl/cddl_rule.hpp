// Copyright 2019
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_CDDL_CDDL_RULE_HPP
#define JSONCONS_CDDL_CDDL_RULE_HPP

#include <jsoncons/json_cursor.hpp>
#include <jsoncons_ext/cddl/cddl_error.hpp>
#include <memory>
#include <unordered_map>

namespace jsoncons { namespace cddl {

class rule_base;

typedef std::unordered_map<std::string,std::unique_ptr<rule_base>> rule_dictionary;

class rule_base
{
public:
    typedef std::unique_ptr<rule_base> unique_ptr;

    rule_base() = default;
    rule_base(const rule_base&) = default;
    rule_base(rule_base&&) = default;
    rule_base& operator=(const rule_base&) = default;
    rule_base& operator=(rule_base&&) = default;

    virtual ~rule_base() = default;

    virtual void validate(const rule_dictionary&, staj_reader&) = 0;
};

class memberkey_rule
{
public:
    memberkey_rule() = default;
    memberkey_rule(const memberkey_rule&) = default;
    memberkey_rule(memberkey_rule&&) = default;
    memberkey_rule& operator=(const memberkey_rule&) = default;
    memberkey_rule& operator=(memberkey_rule&&) = default;

    std::string name;
    rule_base::unique_ptr rule; 
};

class array_rule : public rule_base
{
    std::vector<memberkey_rule> memberkey_rules_;
public:
    array_rule() = default;
    array_rule(const array_rule&) = default;
    array_rule(array_rule&&) = default;
    array_rule& operator=(const array_rule&) = default;
    array_rule& operator=(array_rule&&) = default;

    virtual void validate(const rule_dictionary& dictionary, staj_reader& reader)
    {
        const staj_event& event = reader.current();

        switch (event.event_type())
        {
            case staj_event_type::begin_array:
                break;
            default:
                throw std::runtime_error("Expected array");
                break;
        }
        for (size_t i = 0; i < memberkey_rules_.size(); ++i)
        {
            memberkey_rules_[i].rule->validate(dictionary, reader);
        }
    }
};

}}

#endif
