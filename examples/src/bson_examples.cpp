// Copyright 2017 Daniel Parker
// Distributed under Boost license

#include <jsoncons/json.hpp>
#include <jsoncons_ext/bson/bson.hpp>
#include <string>
#include <iomanip>
#include <cassert>

using namespace jsoncons;

namespace
{
    void encode_to_bson()
    {
        std::vector<uint8_t> buffer;
        bson::bson_bytes_encoder encoder(buffer);
        encoder.begin_array(); // The total number of bytes comprising 
                              // the bson document will be calculated
        encoder.string_value("cat");
        std::vector<uint8_t> purr = {'p','u','r','r'};
        encoder.byte_string_value(purr,7);
        encoder.int64_value(1431027667, semantic_tag::timestamp);
        encoder.end_array();
        encoder.flush();

        std::cout << byte_string_view(buffer) << "\n\n";

    /* 
        23000000 -- Total number of bytes comprising the document (35 bytes) 
          02 -- UTF-8 string
            3000 -- "0"
            04000000 -- number bytes in the string (including trailing byte)
              636174  -- "cat"
                00 -- trailing byte
          05 -- binary
            3100 -- "1"
            04000000 -- number of bytes
            07 -- subtype
            70757272 -- 'P','u','r','r'
          09 -- datetime
          3200 -- "2"
            d3bf4b55 -- 1431027667
          00 
    */ 
    }

    void subtype_example()
    {
        // Create some bson
        std::vector<uint8_t> buffer;
        bson::bson_bytes_encoder encoder(buffer);
        encoder.begin_object(); // The total number of bytes comprising 
                                // the bson document will be calculated
        encoder.key("Hello");
        encoder.string_value("World");
        encoder.key("Data");
        std::vector<uint8_t> bstr = {'f','o','o','b','a','r'};
        encoder.byte_string_value(bstr, 0x80); // binary with subtype 0x80
        encoder.end_object();
        encoder.flush();

        std::cout << "(1)\n" << byte_string_view(buffer) << "\n";

        /*
            0x27,0x00,0x00,0x00, // Total number of bytes comprising the document (40 bytes) 
                0x02, // URF-8 string
                    0x48,0x65,0x6c,0x6c,0x6f, // Hello
                    0x00, // trailing byte 
                0x06,0x00,0x00,0x00, // Number bytes in string (including trailing byte)
                    0x57,0x6f,0x72,0x6c,0x64, // World
                    0x00, // trailing byte
                0x05, // binary
                    0x44,0x61,0x74,0x61, // Data
                    0x00, // trailing byte
                0x06,0x00,0x00,0x00, // number of bytes
                    0x80, // subtype
                    0x66,0x6f,0x6f,0x62,0x61,0x72,
            0x00
        */

        ojson j = bson::decode_bson<ojson>(buffer);

        std::cout << "(2)\n" << pretty_print(j) << "\n\n";
        std::cout << "(3) " << j["Data"].tag() << "("  << j["Data"].ext_tag() << ")\n\n";

        // Get binary value as a std::vector<uint8_t>
        auto bstr2 = j["Data"].as<std::vector<uint8_t>>();
        assert(bstr2 == bstr);

        std::vector<uint8_t> buffer2;
        bson::encode_bson(j, buffer2);
        assert(buffer2 == buffer);
    }
}

void bson_examples()
{
    std::cout << "\nbson examples\n\n";
    encode_to_bson();
    subtype_example();
    std::cout << std::endl;
}

