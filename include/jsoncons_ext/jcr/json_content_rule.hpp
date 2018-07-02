// Copyright 2013 Daniel Parkerjson_input_handler
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_JCR_JSONONTENTRULE_HPP
#define JSONCONS_JCR_JSONONTENTRULE_HPP

#include <string>
#include <jsoncons/json_exception.hpp>
#include <jsoncons/jsoncons_utilities.hpp>

namespace jsoncons {
    class serializing_context;
}

namespace jsoncons { namespace jcr {

class json_content_rule
{
public:
    typedef std::shared_ptr<json_content_rule> shared_ptr;

    virtual ~json_content_rule() = default;
};

}}

#endif
