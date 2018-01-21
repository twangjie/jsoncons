// Copyright 2013 Daniel Parkerjson_input_handler
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_JCR_JCR_INPUT_HANDLER_HPP
#define JSONCONS_JCR_JCR_INPUT_HANDLER_HPP

#include <string>
#include <jsoncons/json_exception.hpp>
#include <jsoncons/jsoncons_utilities.hpp>
#include <jsoncons_ext/jcr/json_content_rule.hpp>

namespace jsoncons {
    class parsing_context;
}

namespace jsoncons { namespace jcr {

template <class CharT>
class basic_jcr_input_handler
{
public:
    typedef CharT char_type;
    typedef std::char_traits<char_type> char_traits_type;

    typedef basic_string_view_ext<char_type,char_traits_type> string_view_type;

    virtual ~basic_jcr_input_handler() {}

    void begin_json()
    {
        do_begin_json();
    }

    void end_json()
    {
        do_end_json();
    }

    void begin_object(const parsing_context& context)
    {
        do_begin_object(context);
    }

    void end_object(const parsing_context& context)
    {
        do_end_object(context);
    }

    void begin_array(const parsing_context& context)
    {
        do_begin_array(context);
    }

    void end_array(const parsing_context& context)
    {
        do_end_array(context);
    }

    void name(const string_view_type& name, const parsing_context& context)
    {
        do_name(name, context);
    }

    void rule(json_content_rule::shared_ptr rule, const parsing_context& context) 
    {
        do_rule(rule, context);
    }

    void string_value(const string_view_type& value, const parsing_context& context) 
    {
        do_string_value(value, context);
    }

    void bool_value(bool value, const parsing_context& context) 
    {
        do_bool_value(value,context);
    }

    void null_value(const parsing_context& context) 
    {
        do_null_value(context);
    }

private:
    virtual void do_begin_json() = 0;

    virtual void do_end_json() = 0;

    virtual void do_begin_object(const parsing_context& context) = 0;

    virtual void do_end_object(const parsing_context& context) = 0;

    virtual void do_begin_array(const parsing_context& context) = 0;

    virtual void do_end_array(const parsing_context& context) = 0;

    virtual void do_name(const string_view_type& name, const parsing_context& context) = 0;

    virtual void do_null_value(const parsing_context& context) = 0;

    virtual void do_string_value(const string_view_type& value, const parsing_context& context) = 0;

    virtual void do_rule(json_content_rule::shared_ptr rule, const parsing_context& context) = 0;

    virtual void do_bool_value(bool value, const parsing_context& context) = 0;
};

template <class CharT>
class basic_null_jcr_input_handler : public basic_jcr_input_handler<CharT>
{
public:
    using typename basic_jcr_input_handler<CharT>::string_view_type                                 ;
private:
    void do_begin_json() override
    {
    }

    void do_end_json() override
    {
    }

    void do_begin_object(const parsing_context&) override
    {
    }

    void do_end_object(const parsing_context&) override
    {
    }

    void do_begin_array(const parsing_context&) override
    {
    }

    void do_end_array(const parsing_context&) override
    {
    }

    void do_name(const string_view_type&, const parsing_context&) override
    {
    }

    void do_null_value(const parsing_context&) override
    {
    }

    void do_string_value(const string_view_type&, const parsing_context&) override
    {
    }

    void do_rule(json_content_rule::shared_ptr rule, const parsing_context&) override
    {
    }

    void do_bool_value(bool, const parsing_context&) override
    {
    }
};

typedef basic_jcr_input_handler<char> jcr_input_handler;
typedef basic_jcr_input_handler<wchar_t> wjcr_input_handler;

typedef basic_null_jcr_input_handler<char> null_jcr_input_handler;
typedef basic_null_jcr_input_handler<wchar_t> wnull_jcr_input_handler;

}}

#endif
