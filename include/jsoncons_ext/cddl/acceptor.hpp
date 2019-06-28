// Copyright 2019
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_CDDL_ACCEPTOR_HPP
#define JSONCONS_CDDL_ACCEPTOR_HPP

#include <jsoncons/staj_reader.hpp>
#include <jsoncons_ext/cddl/cddl_error.hpp>
#include <memory>
#include <unordered_map>
#include <jsoncons_ext/cddl/rule.hpp>

namespace jsoncons { namespace cddl {

    struct acceptor
    {
        typedef json json_type;
        group_entry_rule grpent_rule;   

        acceptor(const group_entry_rule& grpent_rule)
            : grpent_rule(grpent_rule)
        {
        }
        acceptor(const acceptor&) = default;
        acceptor(acceptor&&) = default;

        rule_base* rule()
        {
            return grpent_rule.rule;
        }
    };

}}

#endif
