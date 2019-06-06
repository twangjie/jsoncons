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

    explicit memberkey_rule(const std::string& key) 
        : key(key), rule(def_rule())
    {
    }
    explicit memberkey_rule(std::string&& key) 
        : key(std::move(key)), rule(def_rule())
    {
    }
    memberkey_rule(const memberkey_rule&) = default;
    memberkey_rule(memberkey_rule&&) = default;
    memberkey_rule& operator=(const memberkey_rule&) = default;
    memberkey_rule& operator=(memberkey_rule&&) = default;
};

class tstr_rule : public rule_base
{
public:
    tstr_rule() = default;
    tstr_rule(const tstr_rule&) = default;
    tstr_rule(tstr_rule&&) = default;
    tstr_rule& operator=(const tstr_rule&) = default;
    tstr_rule& operator=(tstr_rule&&) = default;

    virtual void validate(const rule_dictionary&, staj_reader& reader)
    {
        const staj_event& event = reader.current();

        switch (event.event_type())
        {
            case staj_event_type::string_value:
                std::cout << "tstr\n";
                reader.next();
                break;
            default:
                throw std::runtime_error("Expected text string");
                break;
        }
    }
};

class uint_rule : public rule_base
{
public:
    uint_rule() = default;
    uint_rule(const uint_rule&) = default;
    uint_rule(uint_rule&&) = default;
    uint_rule& operator=(const uint_rule&) = default;
    uint_rule& operator=(uint_rule&&) = default;

    virtual void validate(const rule_dictionary&, staj_reader& reader)
    {
        std::cout << "Expect unsigned integer " << (int)reader.current().event_type() << "\n";
        switch (reader.current().event_type())
        {
            case staj_event_type::uint64_value:
                std::cout << "validate uint\n";
                reader.next();
                break;
            case staj_event_type::int64_value:
                std::cout << "validate int\n";
                reader.next();
                break;
            default:
                throw std::runtime_error("Expected unsigned integer");
                break;
        }
    }
};

class tstr_value_rule : public rule_base
{
    std::string value_;
public:
    tstr_value_rule(const std::string& value) 
        : value_(value)
    {
    }
    tstr_value_rule(std::string&& value) 
        : value_(std::move(value))
    {
    }
    tstr_value_rule(const tstr_value_rule&) = default;
    tstr_value_rule(tstr_value_rule&&) = default;
    tstr_value_rule& operator=(const tstr_value_rule&) = default;
    tstr_value_rule& operator=(tstr_value_rule&&) = default;

    virtual void validate(const rule_dictionary&, staj_reader& reader)
    {
        const staj_event& event = reader.current();

        switch (event.event_type())
        {
            case staj_event_type::string_value:
                std::cout << "tstr_value_rule\n";
                if (reader.current().as<std::string>() != value_)
                {
                    throw std::runtime_error("Expected text string value");
                }
                reader.next();
                break;
            default:
                throw std::runtime_error("Expected text string");
                break;
        }
    }
};

class lookup_rule : public rule_base
{
    std::string value_;
public:
    lookup_rule(const std::string& value) 
        : value_(value)
    {
    }
    lookup_rule(std::string&& value) 
        : value_(std::move(value))
    {
    }
    lookup_rule(const lookup_rule&) = default;
    lookup_rule(lookup_rule&&) = default;
    lookup_rule& operator=(const lookup_rule&) = default;
    lookup_rule& operator=(lookup_rule&&) = default;

    virtual void validate(const rule_dictionary& dictionary, staj_reader& reader)
    {
        auto it = dictionary.find(value_);
        if (it == dictionary.end())
        {
            std::cout << value_ << " NOT FOUND\n";
            throw std::runtime_error("Rule not found");
        }
        (it->second)->validate(dictionary, reader);
    }
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
        switch (reader.current().event_type())
        {
            case staj_event_type::begin_array:
                reader.next();
                break;
            default:
                throw std::runtime_error("Expected array");
                break;
        }
        for (size_t i = 0; i < memberkey_rules_.size(); ++i)
        {
            if (reader.current().event_type() == staj_event_type::end_array)
            {
                break;
            }
            std::cout << "event_type: " << (int)reader.current().event_type() << "\n";
            memberkey_rules_[i].rule->validate(dictionary, reader);
        }
        while (reader.current().event_type() != staj_event_type::end_array)
        {
            reader.next();
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
                reader.next();
                break;
            default:
                throw std::runtime_error("Expected object");
                break;
        }
        for (size_t i = 0; i < memberkey_rules_.size(); ++i)
        {
            if (reader.current().event_type() == staj_event_type::end_object)
            {
                break;
            }
            memberkey_rules_[i].rule->validate(dictionary, reader);
        }
        while (reader.current().event_type() != staj_event_type::end_object)
        {
            reader.next();
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
