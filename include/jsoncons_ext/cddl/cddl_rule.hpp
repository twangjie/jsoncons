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

typedef std::unordered_map<std::string,rule_base*> rule_dictionary;

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

class default_rule : public rule_base
{
public:
    default_rule() = default;
    default_rule(const default_rule&) = default;
    default_rule(default_rule&&) = default;
    default_rule& operator=(const default_rule&) = default;
    default_rule& operator=(default_rule&&) = default;

    virtual ~default_rule() = default;

    virtual void validate(const rule_dictionary&, staj_reader&)
    {
        std::cout << "default validate \n";
    }
};

class memberkey_rule
{
    static default_rule* def_rule()
    {
        static default_rule adefault;
        return &adefault;
    }
public:
    std::string key;
    rule_base* rule; 

    memberkey_rule() 
    {
        rule = def_rule();
    }
    memberkey_rule(const std::string& key) 
        : key(key), rule(def_rule())
    {
    }
    memberkey_rule(std::string&& key) 
        : key(std::move(key)), rule(def_rule())
    {
    }
    memberkey_rule(const memberkey_rule&) = default;
    memberkey_rule(memberkey_rule&&) = default;
    memberkey_rule& operator=(const memberkey_rule&) = default;
    memberkey_rule& operator=(memberkey_rule&&) = default;
};

class structure_rule : public rule_base
{
public:
    structure_rule() = default;
    structure_rule(const structure_rule&) = default;
    structure_rule(structure_rule&&) = default;
    structure_rule& operator=(const structure_rule&) = default;
    structure_rule& operator=(structure_rule&&) = default;

    std::vector<memberkey_rule> memberkey_rules_;
};

class array_rule : public structure_rule
{
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

class map_rule : public structure_rule
{
public:
    map_rule() = default;
    map_rule(const map_rule&) = default;
    map_rule(map_rule&&) = default;
    map_rule& operator=(const map_rule&) = default;
    map_rule& operator=(map_rule&&) = default;

    virtual void validate(const rule_dictionary& dictionary, staj_reader& reader)
    {
        const staj_event& event = reader.current();

        switch (event.event_type())
        {
            case staj_event_type::begin_object:
                break;
            default:
                throw std::runtime_error("Expected object");
                break;
        }
        for (size_t i = 0; i < memberkey_rules_.size(); ++i)
        {
            memberkey_rules_[i].rule->validate(dictionary, reader);
        }
    }
};

class group_rule : public structure_rule
{
public:
    group_rule() = default;
    group_rule(const group_rule&) = default;
    group_rule(group_rule&&) = default;
    group_rule& operator=(const group_rule&) = default;
    group_rule& operator=(group_rule&&) = default;

    virtual void validate(const rule_dictionary& dictionary, staj_reader& reader)
    {
        for (size_t i = 0; i < memberkey_rules_.size(); ++i)
        {
            memberkey_rules_[i].rule->validate(dictionary, reader);
        }
    }
};

}}

#endif
