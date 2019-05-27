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
    }
};

typedef basic_cddl_validator<char> cddl_validator;

}}

#endif
