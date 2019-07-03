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

        rule_base* parent_;
        bool is_root_;
        size_t index_;
        size_t occur_;

        acceptor(rule_base* parent, bool is_root = true)
            : parent_(parent), is_root_(is_root), index_(0), occur_(0)
        {
        }
        acceptor(const acceptor&) = default;
        acceptor(acceptor&&) = default;

        acceptor make_child_acceptor(std::string& key, staj_event_type ev_type) const
        {  
            if (is_root_)
            {
                if (!parent_->accept_event_type(ev_type))
                {
                    throw std::runtime_error("Unmatched type");
                }
            }
            else
            {
            }
            return acceptor(parent_, false);
        }

        bool accept_event(const std::string& key, const staj_event& ev) 
        {
            bool result = false;
            if (parent_->is_array())
            {
                if (index_ < parent_->size())
                {
                    auto group_ent = parent_->at(index_);
                    if (occur_ < group_ent.max_occur)
                    {
                        result = group_ent.rule->accept_event(ev);
                        ++occur_;
                    }
                    if (!result)
                    {
                        if (occur_ >= group_ent.min_occur && index_+1 < parent_->size())
                        {
                            ++index_;
                            occur_ = 0;
                            auto group_ent = parent_->at(index_);
                            result = group_ent.rule->accept_event(ev);
                            ++occur_;
                        }
                    }
                }
            }
            else 
            {
            }
            return parent_->accept_event(ev);
        }
    };

}}

#endif
