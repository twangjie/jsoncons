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
#include <jsoncons/json_pull_reader.hpp>
#include <jsoncons_ext/cddl/cddl.hpp>

using namespace jsoncons;

namespace jsoncons { namespace cddl {

}}

TEST_CASE("cddl tests")
{
    std::string root = R"(
    [
        "city" : "Toronto",
        {"longitude" : 100, "latitude" : 100}
    ]
    )";

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

        std::unique_ptr<cddl::cddl_specification> spec = cddl::cddl_specification::parse(s);

        cddl::cddl_validator validator(std::move(spec)); 

        json_pull_reader reader(root);
        validator.validate(reader);
    }
}
#if 0
TEST_CASE("cddl tests 2")
{
    SECTION("test 2")
    {
        std::string s = R"( 
            pii = (
                      age: int,
                      name: tstr,
                      employer: tstr,
                   )

            person = {pii}       
        )";

        cddl::cddl_specification spec = cddl::cddl_specification::parse(s);

        //cddl::cddl_validator cddl_validator(std::move(schema)); 
        //cddl::validate(cddl_validator, "42");
    }
    SECTION("test 3")
    {
        std::string s = R"( 
            attire = "bow tie" / "necktie" / "Internet attire"
            protocol = 6 / 17
        )";

        cddl::cddl_specification spec = cddl::cddl_specification::parse(s);

        //cddl::cddl_validator cddl_validator(std::move(schema)); 
        //cddl::validate(cddl_validator, "42");
    }

    SECTION("test 4")
    {
        std::string s = R"( 
            attire = "bow tie" / "necktie" / "Internet attire"
            protocol = 6 .. 17
            age = 18 ... 30
        )";

        cddl::cddl_specification spec = cddl::cddl_specification::parse(s);

        //cddl::cddl_validator cddl_validator(std::move(schema)); 
        //cddl::validate(cddl_validator, "42");
    }

    SECTION("test 5")
    {
        std::string s = R"(person = {(
                                   age: int,
                                   name: tstr,
                                   employer: tstr,
                             )})";

        cddl::cddl_specification spec = cddl::cddl_specification::parse(s);

        //cddl::cddl_validator cddl_validator(std::move(schema)); 
        //cddl::validate(cddl_validator, "42");
    } 

    SECTION("test 6")
    {
        std::string s = R"(
        person = {
              identity,
              employer: tstr,
            }

            dog = {
              identity,
              leash-length: float,
            }

            identity = (
              age: int,
              name: tstr,
            )
        )";

        cddl::cddl_specification spec = cddl::cddl_specification::parse(s);

        //cddl::cddl_validator cddl_validator(std::move(schema)); 
        //cddl::validate(cddl_validator, "42");
    } 

    SECTION("Occurrence *")
    {
        std::string s = R"(
            apartment = {
                kitchen: size,
                * bedroom: size,
            }
            size = float ; in m2        
        )";

        cddl::cddl_specification spec = cddl::cddl_specification::parse(s);

        //cddl::cddl_validator cddl_validator(std::move(schema)); 
        //cddl::validate(cddl_validator, "42");
    } 
    SECTION("Occurrence ?")
    {
        std::string s = R"(
            apartment = {
                kitchen: size,
                ? bedroom: size,
            }
            size = float ; in m2        
        )";

        cddl::cddl_specification spec = cddl::cddl_specification::parse(s);

        //cddl::cddl_validator cddl_validator(std::move(schema)); 
        //cddl::validate(cddl_validator, "42");
    } 
    SECTION("Arrays")
    {
        std::string s = R"(
            unlimited-people = [* person]
            one-or-two-people = [1*2 person]
            at-least-two-people = [2* person]
            person = (
                name: tstr,
                age: uint,
            )        
        )";

        cddl::cddl_specification spec = cddl::cddl_specification::parse(s);

        //cddl::cddl_validator cddl_validator(std::move(schema)); 
        //cddl::validate(cddl_validator, "42");
    } 
}
#endif
