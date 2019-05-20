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
#if 0
    SECTION("test 1")
    {
        std::string s = R"( 
        Geography = [
         city           : tstr,
         gpsCoordinates : GpsCoordinates,
       ]

       GpsCoordinates = {
         longitude      : uint,            ; degrees, scaled by 10^7
         latitude       : uint,            ; degreed, scaled by 10^7
       }
       )";

        cddl::cddl_specification spec = cddl::cddl_specification::parse(s);

        //cddl::validator validator(std::move(schema)); 
        //cddl::validate(validator, "42");
    }
    SECTION("test 2")
    {
        std::string s = R"( 
            pii = (
                      age: int,
                      name: tstr,
                      employer: tstr,
                   )

            person = {
                         pii
                     }       
        )";

        cddl::cddl_specification spec = cddl::cddl_specification::parse(s);

        //cddl::validator validator(std::move(schema)); 
        //cddl::validate(validator, "42");
    }
    SECTION("test 3")
    {
        std::string s = R"( 
            attire = "bow tie" / "necktie" / "Internet attire"
            protocol = 6 / 17
        )";

        cddl::cddl_specification spec = cddl::cddl_specification::parse(s);

        //cddl::validator validator(std::move(schema)); 
        //cddl::validate(validator, "42");
    }
#endif
    SECTION("test 4")
    {
        std::string s = R"( 
            attire = "bow tie" / "necktie" / "Internet attire"
            protocol = 6 .. 17
            age = 18 ... 30
        )";

        cddl::cddl_specification spec = cddl::cddl_specification::parse(s);

        //cddl::validator validator(std::move(schema)); 
        //cddl::validate(validator, "42");
    }



    /* SECTION("test 2")
    {
        std::string s = R"(person = {(
                                   age: int,
                                   name: tstr,
                                   employer: tstr,
                             )})";

        cddl::cddl_specification spec = cddl::cddl_specification::parse(s);

        //cddl::validator validator(std::move(schema)); 
        //cddl::validate(validator, "42");
    } */

}

