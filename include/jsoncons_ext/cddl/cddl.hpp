// Copyright 2019 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_CDDL_CDDL_HPP
#define JSONCONS_CDDL_CDDL_HPP

#include <jsoncons/json.hpp>
#include <jsoncons_ext/cddl/cddl_specification.hpp>

namespace jsoncons { namespace cddl {

template <class CharT>
class basic_cddl_validator
{
    basic_cddl_specification<CharT> spec_;
public:
    typedef CharT char_type;
    typedef typename basic_json_content_handler<char_type>::string_view_type string_view_type;

    basic_cddl_validator(const basic_cddl_specification<CharT>& schema)
        : spec_(schema)
    {
    }
    basic_cddl_validator(basic_cddl_specification<CharT>&& schema)
        : spec_(std::move(schema))
    {
    }
    basic_cddl_validator(const basic_cddl_validator&) = default;
    basic_cddl_validator(basic_cddl_validator&&) = default;

    basic_cddl_validator& operator=(const basic_cddl_validator&) = default;
    basic_cddl_validator& operator=(basic_cddl_validator&&) = default;

    bool is_valid() const
    {
        return true;
    }

    void validate(basic_staj_reader<CharT>& reader)
    {
        const basic_staj_event<CharT>& event = reader.current();
         
        switch (event.event_type())
        {
            case staj_event_type::begin_array:
                std::cout << "begin_array\n";
                break;
            case staj_event_type::end_array:
                std::cout << "end_array\n";
                break;
            case staj_event_type::begin_object:
                std::cout << "begin_object\n";
                break;
            case staj_event_type::end_object:
                std::cout << "end_object\n";
                break;
            case staj_event_type::name:
                // If underlying type is string, can return as string_view
                std::cout << "name: " << event.as<jsoncons::string_view>() << "\n";
                break;
            case staj_event_type::string_value:
                std::cout << "string_value: " << event.as<jsoncons::string_view>() << "\n";
                break;
            case staj_event_type::byte_string_value:
                //std::cout << "byte_string_value: " << event.as<jsoncons::byte_string>() << "\n";
                break;
            case staj_event_type::null_value:
                std::cout << "null_value: " << event.as<std::string>() << "\n";
                break;
            case staj_event_type::bool_value:
                std::cout << "bool_value: " << event.as<std::string>() << "\n";
                break;
            case staj_event_type::int64_value:
                std::cout << "int64_value: " << event.as<std::string>() << "\n";
                break;
            case staj_event_type::uint64_value:
                std::cout << "uint64_value: " << event.as<std::string>() << "\n";
                break;
            case staj_event_type::double_value:
                // Return as string, could also use event.as<double>()
                std::cout << "double_value: " << event.as<std::string>() << "\n";
                break;
            default:
                std::cout << "Unhandled event type\n";
                break;
        }
    }
};

typedef basic_cddl_validator<char> cddl_validator;

}}

#endif
