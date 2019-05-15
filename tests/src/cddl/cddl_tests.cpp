// Copyright 2019 Daniel Parker
// Distributed under Boost license

#if defined(_MSC_VER)
#include "windows.h" // test no inadvertant macro expansions
#endif
#include <catch/catch.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <utility>
#include <ctime>
#include <new>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/cddl/cddl.hpp>

using namespace jsoncons;

namespace jsoncons { namespace cddl {

void validate(cddl::validator& validator, const std::string& jtext)    
{
    json j = json::parse(jtext);
    j.dump(validator);
}

}}

TEST_CASE("cddl tests")
{
    std::string s = R"(person = {(
                               age: int,
                               name: tstr,
                               employer: tstr,
                             )})";

    cddl::cddl_specification spec = cddl::cddl_specification::parse(s);

    //cddl::validator validator(std::move(schema)); 
    //cddl::validate(validator, "42");

}

