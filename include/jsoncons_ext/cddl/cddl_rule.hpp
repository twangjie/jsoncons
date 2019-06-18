// Copyright 2019
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_CDDL_CDDL_RULE_HPP
#define JSONCONS_CDDL_CDDL_RULE_HPP

#include <jsoncons/staj_reader.hpp>
#include <jsoncons_ext/cddl/cddl_error.hpp>
#include <memory>
#include <unordered_map>

namespace jsoncons { namespace cddl {

class rule_base;

class group_entry_rule
{
    static rule_base* def_rule();
public:
    size_t min_occur;
    size_t max_occur;
    std::string key;
    rule_base* rule; 

    group_entry_rule(rule_base* rule) 
        : rule(rule)
    {
    }
    group_entry_rule(size_t min_occur, size_t max_occur) 
        : min_occur(min_occur), max_occur(max_occur), rule(def_rule())
    {
    }
    group_entry_rule(const group_entry_rule&) = default;
    group_entry_rule(group_entry_rule&&) = default;
    group_entry_rule& operator=(const group_entry_rule&) = default;
    group_entry_rule& operator=(group_entry_rule&&) = default;
};

typedef std::unordered_map<std::string,rule_base*> rule_dictionary;

class rule_base
{
public:
    typedef std::unique_ptr<rule_base> unique_ptr;

    static const size_t unbounded = size_t(-1);

    rule_base() = default;
    rule_base(const rule_base&) = default;
    rule_base(rule_base&&) = default;
    rule_base& operator=(const rule_base&) = default;
    rule_base& operator=(rule_base&&) = default;

    virtual ~rule_base() = default;

    virtual cddl_errc validate(const rule_dictionary&, staj_reader&) = 0;

    virtual bool matches_event(const staj_event& event) = 0;
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

    bool matches_event(const staj_event& event) override
    {
        return false;
    }
};


class tstr_rule : public rule_base
{
public:
    tstr_rule() = default;
    tstr_rule(const tstr_rule&) = default;
    tstr_rule(tstr_rule&&) = default;
    tstr_rule& operator=(const tstr_rule&) = default;
    tstr_rule& operator=(tstr_rule&&) = default;

    cddl_errc validate(const rule_dictionary&, staj_reader& reader) override
    {
        const staj_event& event = reader.current();

        switch (event.event_type())
        {
            case staj_event_type::string_value:
                std::cout << "tstr\n";
                reader.next();
                break;
            default:
                return cddl_errc::expected_tstr;
        }
        return cddl_errc{};
    }

    bool matches_event(const staj_event& event) override
    {
        switch (event.event_type())
        {
        case staj_event_type::string_value:
                return true;
            default:
                return false;
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

    virtual cddl_errc validate(const rule_dictionary&, staj_reader& reader)
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
                return cddl_errc::expected_uint;
                break;
        }
        return cddl_errc{};
    }

    bool matches_event(const staj_event& event) override
    {
        switch (event.event_type())
        {
            case staj_event_type::uint64_value:
                return true;
            case staj_event_type::int64_value:
                return true;
            default:
                return false;
        }
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
                return cddl_errc::expected_int;
                break;
        }
        return cddl_errc{};
    }

    bool matches_event(const staj_event& event) override
    {
        return false;
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
        std::cout << "Expect unsigned integer " << (int)reader.current().event_type() << "\n";
        switch (reader.current().event_type())
        {
            case staj_event_type::double_value:
                std::cout << "validate double\n";
                reader.next();
                break;
            default:
                return cddl_errc::expected_float;
        }
        return cddl_errc{};
    }

    bool matches_event(const staj_event& event) override
    {
        return false;
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
        const staj_event& event = reader.current();

        switch (event.event_type())
        {
            case staj_event_type::string_value:
                std::cout << "tstr_value_rule\n";
                if (reader.current().as<std::string>() != value_)
                {
                    return cddl_errc::expected_tstr;
                }
                else
                {
                    reader.next();
                }
                break;
            default:
                return cddl_errc::expected_tstr;
        }
        return cddl_errc{};
    }

    bool matches_event(const staj_event& event) override
    {
        return false;
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
        auto it = dictionary.find(value_);
        if (it == dictionary.end())
        {
            std::cout << value_ << " NOT FOUND\n";
            return cddl_errc::id_lookup_failed;
        }
        (it->second)->validate(dictionary, reader);
        return cddl_errc{};
    }

    bool matches_event(const staj_event& event) override
    {
        return false;
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

    std::vector<group_entry_rule> memberkey_rules_;

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
        switch (reader.current().event_type())
        {
            case staj_event_type::begin_array:
                reader.next();
                break;
            default:
                std::cout << "Expected array, found " << (int)reader.current().event_type() << "\n";  
                return cddl_errc::expected_array;
        }

        size_t i = 0;

        for (;i < memberkey_rules_.size();)
        {
            if (reader.current().event_type() == staj_event_type::end_array)
            {
                break;
            }
            std::cout << "event_type: " << (int)reader.current().event_type() << "\n";
            cddl_errc result = memberkey_rules_[i].rule->validate(dictionary, reader);
            if (result != cddl_errc())
            {
                if (memberkey_rules_[i].min_occur == 0)
                {
                    ++i;
                }
                else
                {

                }
            }
            ++i;
        }
        while (!reader.done() && reader.current().event_type() != staj_event_type::end_array)
        {
            reader.next();
        }

        if (reader.current().event_type() == staj_event_type::end_array)
        {
            reader.next();
        }
        return cddl_errc{};
    }

    bool is_array() const override
    {
        return true;
    }

    bool matches_event(const staj_event& event) override
    {
        switch (event.event_type())
        {
            case staj_event_type::begin_array:
                return true;
            default:
                return false;
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

    virtual cddl_errc validate(const rule_dictionary& dictionary, staj_reader& reader)
    {
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
                return cddl_errc::expected_map;
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
        return cddl_errc{};
    }

    bool is_map() const override
    {
        return true;
    }

    bool matches_event(const staj_event& event) override
    {
        switch (event.event_type())
        {
            case staj_event_type::begin_object:
                return true;
            default:
                return false;
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

    virtual cddl_errc validate(const rule_dictionary& dictionary, staj_reader& reader)
    {
        for (size_t i = 0; i < memberkey_rules_.size(); ++i)
        {
            cddl_errc result = memberkey_rules_[i].rule->validate(dictionary, reader);
            if (result != cddl_errc())
            {
                return result;
            }
        }
        return cddl_errc{};
    }
    bool matches_event(const staj_event& event) override
    {
        switch (event.event_type())
        {
            case staj_event_type::begin_object:
                return false;
            default:
                return false;
        }
    }

    bool is_group() const override
    {
        return true;
    }
};

rule_base* group_entry_rule::def_rule()
{
    static default_rule adefault;
    return &adefault;
}

}}

#endif
