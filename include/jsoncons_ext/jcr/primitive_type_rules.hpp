// Copyright 2013 Daniel Parkerjson_input_handler
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_JCR_PRIMITIVETYPERULES_HPP
#define JSONCONS_JCR_PRIMITIVETYPERULES_HPP

#include <string>
#include <jsoncons/json_exception.hpp>
#include <jsoncons/jsoncons_utilities.hpp>
#include <jsoncons_ext/jcr/json_content_rule.hpp>

namespace jsoncons {
    class parsing_context;
}

namespace jsoncons { namespace jcr {

// integer rules

class specific_integer_rule : public virtual json_content_rule
{
    int64_t value_;
public:
    specific_integer_rule(int64_t value)
        : value_(value)
    {
    }
};

class specific_uinteger_rule : public virtual json_content_rule
{
    uint64_t value_;
public:
    specific_uinteger_rule(uint64_t value)
        : value_(value)
    {
    }
};

class integer_rule : public virtual json_content_rule
{
public:
    integer_rule()
    {
    }
};

// double rules

class specific_double_rule : public virtual json_content_rule
{
    double value_;
    uint8_t precision_;
public:
    specific_double_rule(double value, uint8_t precision)
        : value_(value), precision_(precision)
    {
    }
};


class double_rule : public virtual json_content_rule
{
public:
    double_rule()
    {
    }
};

}}

#endif
