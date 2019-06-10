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

enum class occurence_type {exactly_once,optional,zero_or_more,one_or_more};

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

    virtual cddl_errc validate(const rule_dictionary&, staj_reader&) = 0;
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

    virtual cddl_errc validate(const rule_dictionary&, staj_reader&)
    {
        std::cout << "default validate \n";
        return cddl_errc();
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
    occurence_type occur;
    std::string key;
    rule_base* rule; 

    memberkey_rule() 
        : rule(def_rule())
    {
    }
    explicit memberkey_rule(const std::string& key) 
        : occur(occurence_type::exactly_once), key(key), rule(def_rule())
    {
    }
    explicit memberkey_rule(std::string&& key) 
        : occur(occurence_type::exactly_once), key(std::move(key)), rule(def_rule())
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

    virtual cddl_errc validate(const rule_dictionary&, staj_reader& reader)
    {
        cddl_errc result{};

        const staj_event& event = reader.current();

        switch (event.event_type())
        {
            case staj_event_type::string_value:
                std::cout << "tstr\n";
                reader.next();
                break;
            default:
                result = cddl_errc::expected_tstr;
                break;
        }
        return result;
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

    virtual cddl_errc validate(const rule_dictionary&, staj_reader& reader)
    {
        cddl_errc result{};
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
                result = cddl_errc::expected_uint;
                break;
        }
        return result;
    }
};

class int_rule : public rule_base
{
public:
    int_rule() = default;
    int_rule(const int_rule&) = default;
    int_rule(int_rule&&) = default;
    int_rule& operator=(const int_rule&) = default;
    int_rule& operator=(int_rule&&) = default;

    virtual cddl_errc validate(const rule_dictionary&, staj_reader& reader)
    {
        cddl_errc result{};
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
                result = cddl_errc::expected_int;
                break;
        }
        return result;
    }
};

class float_rule : public rule_base
{
public:
    float_rule() = default;
    float_rule(const float_rule&) = default;
    float_rule(float_rule&&) = default;
    float_rule& operator=(const float_rule&) = default;
    float_rule& operator=(float_rule&&) = default;

    virtual cddl_errc validate(const rule_dictionary&, staj_reader& reader)
    {
        cddl_errc result{};
        std::cout << "Expect unsigned integer " << (int)reader.current().event_type() << "\n";
        switch (reader.current().event_type())
        {
            case staj_event_type::double_value:
                std::cout << "validate double\n";
                reader.next();
                break;
            default:
                result = cddl_errc::expected_float;
                break;
        }
        return result;
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

    virtual cddl_errc validate(const rule_dictionary&, staj_reader& reader)
    {
        cddl_errc result{};
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
        return result;
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

    virtual cddl_errc validate(const rule_dictionary& dictionary, staj_reader& reader)
    {
        cddl_errc result{};
        auto it = dictionary.find(value_);
        if (it == dictionary.end())
        {
            std::cout << value_ << " NOT FOUND\n";
            throw std::runtime_error("Rule not found");
        }
        (it->second)->validate(dictionary, reader);
        return result;
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

    virtual bool is_array() const
    {
        return false;
    }
    virtual bool is_map() const
    {
        return false;
    }
    virtual bool is_group() const
    {
        return false;
    }
};

class array_rule : public structure_rule
{
public:
    array_rule() = default;
    array_rule(const array_rule&) = default;
    array_rule(array_rule&&) = default;
    array_rule& operator=(const array_rule&) = default;
    array_rule& operator=(array_rule&&) = default;

    virtual cddl_errc validate(const rule_dictionary& dictionary, staj_reader& reader)
    {
        cddl_errc result{};
        switch (reader.current().event_type())
        {
            case staj_event_type::begin_array:
                reader.next();
                break;
            default:
                std::cout << "Expected array, found " << (int)reader.current().event_type() << "\n";  
                throw std::runtime_error("Expected array");
                break;
        }

        size_t i = 0;
        for (;i < memberkey_rules_.size(); ++i)
        {
            if (reader.current().event_type() == staj_event_type::end_array)
            {
                break;
            }
            std::cout << "event_type: " << (int)reader.current().event_type() << "\n";
            memberkey_rules_[i].rule->validate(dictionary, reader);
        }
        while (!reader.done() && reader.current().event_type() != staj_event_type::end_array)
        {
            reader.next();
        }
        return result;
    }

    bool is_array() const override
    {
        return true;
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

    virtual cddl_errc validate(const rule_dictionary& dictionary, staj_reader& reader)
    {
        cddl_errc result{};
        std::unordered_map<std::string,rule_base*> rules;
        for (auto& item : memberkey_rules_)
        {
            rules.try_emplace(item.key,item.rule);
        }

        switch (reader.current().event_type())
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
            std::cout << "Expect name: " << (int)reader.current().event_type() << "\n";
            if (reader.current().event_type() != staj_event_type::name)
            {
                throw std::runtime_error("Expected name");
            }
            auto it = rules.find(reader.current().as<std::string>());
            if (it != rules.end())
            {
                reader.next();
                it->second->validate(dictionary, reader);
            }

        }
        while (reader.current().event_type() != staj_event_type::end_object)
        {
            reader.next();
        }
        return result;
    }

    bool is_map() const override
    {
        return true;
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

    virtual cddl_errc validate(const rule_dictionary& dictionary, staj_reader& reader)
    {
        cddl_errc result{};

        for (size_t i = 0; i < memberkey_rules_.size(); ++i)
        {
            memberkey_rules_[i].rule->validate(dictionary, reader);
        }
        return result;
    }

    bool is_group() const override
    {
        return true;
    }
};

}}

#endif
