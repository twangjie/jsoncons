// Copyright 2013 Daniel Parker
// Distributed under Boost license

#ifdef __linux__
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <sstream>
#include <vector>
#include <map>
#include <utility>
#include <ctime>
#include <new>
#include <codecvt>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/jcr/jcr_parser.hpp>

using namespace jsoncons;
using namespace jsoncons::jcr;
using namespace jsoncons::literals;

BOOST_AUTO_TEST_SUITE(jcr_tests)

BOOST_AUTO_TEST_CASE(jcr_test1)
{
    std::string s = R"(
{ "line-count" : 3426, "word-count" : 27886 }
    )";

    null_jcr_input_handler handler;
    jcr_parser parser(s.data(),s.length(),handler);

    std::error_code ec;
    parser.parse(ec);
    BOOST_CHECK(!ec);
}

BOOST_AUTO_TEST_CASE(jcr_test2)
{
    std::string s = R"(
{ "line-count" : integer, "word-count" : integer }
    )";

    null_jcr_input_handler handler;
    jcr_parser parser(s.data(),s.length(),handler);

    std::error_code ec;
    parser.parse(ec);
    if (ec)
    {
        std::cerr << ec.message() << std::endl;
    }
    BOOST_CHECK(!ec);
}

BOOST_AUTO_TEST_CASE(jcr_test3)
{
    std::string s = R"(
{ "line-count" : 0.., "word-count" : -10.., "para-count" : 10.. }
    )";

    null_jcr_input_handler handler;
    jcr_parser parser(s.data(),s.length(),handler);

    std::error_code ec;
    parser.parse(ec);
    if (ec)
    {
        std::cerr << ec.message() << std::endl;
    }
    BOOST_CHECK(!ec);
}

BOOST_AUTO_TEST_SUITE_END()




