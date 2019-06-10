/// Copyright 2018 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_CDDL_CDDL_ERROR_HPP
#define JSONCONS_CDDL_CDDL_ERROR_HPP

#include <system_error>
#include <jsoncons/config/jsoncons_config.hpp>

namespace jsoncons { namespace cddl {

// cddl_spec_errc

enum class cddl_spec_errc
{
    unexpected_eof = 1,
    expected_assign,
    expected_id,
    invalid_id,
    expected_groupent,
    expected_comma_or_left_par_or_right_sqbracket,
    expected_comma_or_left_par_or_right_curbracket,
    expected_comma_or_right_par,
    expected_rangeop_or_slash_or_comma_or_right_bracket,
    invalid_number,
    expected_uint_or_space
};

class cddl_spec_error_category_impl
   : public std::error_category
{
public:
    const char* name() const noexcept override
    {
        return "jsoncons/cddl";
    }
    std::string message(int ev) const override
    {
        switch (static_cast<cddl_spec_errc>(ev))
        {
            case cddl_spec_errc::unexpected_eof:
                return "Unexpected end of file";
            case cddl_spec_errc::expected_assign:
                return "Expected =";
            case cddl_spec_errc::expected_id:
                return "Expected a rule id";
            case cddl_spec_errc::invalid_id:
                return "Invalid rule id";
            case cddl_spec_errc::expected_groupent:
                return "Expected [ or { or (";
            case cddl_spec_errc::expected_comma_or_left_par_or_right_sqbracket:
                return "Expected , or ( or ]";
            case cddl_spec_errc::expected_comma_or_left_par_or_right_curbracket:
                return "Expected , or ( or }";
            case cddl_spec_errc::expected_comma_or_right_par:
                return "Expected , or )";
            case cddl_spec_errc::expected_rangeop_or_slash_or_comma_or_right_bracket:
                return "Expected .. or / or , or right bracket";
            case cddl_spec_errc::invalid_number:
                return "Invalid number";
            case cddl_spec_errc::expected_uint_or_space:
                return "Expected unsigned integer or space";
            default:
                return "Expected unsigned integer or space";
        }
    }
};

inline
const std::error_category& cddl_spec_error_category()
{
  static cddl_spec_error_category_impl instance;
  return instance;
}

inline 
std::error_code make_error_code(cddl_spec_errc result)
{
    return std::error_code(static_cast<int>(result),cddl_spec_error_category());
}

// cddl_error

enum class cddl_errc
{
    expected_tstr = 1,
    expected_uint,
    expected_int,
    expected_float
};

class cddl_error_category_impl
   : public std::error_category
{
public:
    const char* name() const noexcept override
    {
        return "jsoncons/cddl";
    }
    std::string message(int ev) const override
    {
        switch (static_cast<cddl_errc>(ev))
        {
            case cddl_errc::expected_tstr:
                return "Expected text string";
            case cddl_errc::expected_uint:
                return "Expected uint";
            case cddl_errc::expected_int:
                return "Expected int";
            case cddl_errc::expected_float:
                return "Expected float";
            default:
                return "";
        }
    }
};

inline
const std::error_category& cddl_error_category()
{
  static cddl_error_category_impl instance;
  return instance;
}

inline 
std::error_code make_error_code(cddl_errc result)
{
    return std::error_code(static_cast<int>(result),cddl_error_category());
}


}}

namespace std {
    template<>
    struct is_error_code_enum<jsoncons::cddl::cddl_spec_errc> : public true_type
    {
    };

    template<>
    struct is_error_code_enum<jsoncons::cddl::cddl_errc> : public true_type
    {
    };
}

#endif
