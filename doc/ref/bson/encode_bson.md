### jsoncons::bson::encode_bson

Encodes a C++ data structure to the [Binary JSON (BSON)](http://bsonspec.org/) data format.

```c++
#include <jsoncons_ext/bson/bson.hpp>

template<class T>
void encode_bson(const T& jval, std::vector<uint8_t>& v,
                 const bson_decode_options& options = bson_decode_options()); // (1) (until v0.152.0)

template<class T, class Container>
void encode_bson(const T& jval, Container& v,
                 const bson_decode_options& options = bson_decode_options()); // (1) (since v0.152.0)

template<class T>
void encode_bson(const T& jval, std::ostream& os,
                 const bson_decode_options& options = bson_decode_options()); // (2)
```

(1) Writes a value of type T into a byte container in the BSON data format, using the specified (or defaulted) [options](bson_options.md). 
Type 'T' must be an instantiation of [basic_json](basic_json.md) 
or support [json_type_traits](../json_type_traits.md). 
Type `Container` must be back insertable and have member type `value_type` with size exactly 8 bits (since v0.152.0.)
Any of the values types `int8_t`, `uint8_t`, `char`, `unsigned char` and `std::byte` (since C++17) are allowed.

(2) Writes a value of type T into a binary stream in the BSON data format, using the specified (or defaulted) [options](bson_options.md). 
Type 'T' must be an instantiation of [basic_json](basic_json.md) 
or support [json_type_traits](../json_type_traits.md). 

### See also

[decode_bson](decode_bson) decodes a [Bin�ary JSON](http://bsonspec.org/) data format to a json value.

