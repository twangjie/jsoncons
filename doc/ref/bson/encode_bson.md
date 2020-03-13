### jsoncons::bson::encode_bson

Encodes a C++ data structure to the [Binary JSON (BSON)](http://bsonspec.org/) data format.

```c++
#include <jsoncons_ext/bson/bson.hpp>

template<class T>
void encode_bson(const T& jval, std::vector<uint8_t>& v); // (1)

template<class T>
void encode_bson(const T& jval, std::ostream& os); // (2)
```

template<class T,class TempAllocator>
void encode_bson(temp_allocator_arg_t, const TempAllocator& temp_alloc,
                 const T& jval, std::vector<uint8_t>& v); // (3)

template<class T,class TempAllocator>
void encode_bson(temp_allocator_arg_t, const TempAllocator& temp_alloc,
                 const T& jval, std::ostream& os); // (4)
```

(1) Writes a value of type T into a bytes buffer in the BSON data format. Type T must be an instantiation of [basic_json](../basic_json.md) 
or support [json_type_traits](../json_type_traits.md). 

(2) Writes a value of type T into a binary stream in the BSON data format. Type T must be an instantiation of [basic_json](../basic_json.md) 
or support [json_type_traits](../json_type_traits.md). 

Functions (3)-(4) are the same except `temp_alloc` is used to allocate temporary work areas.

### See also

- [decode_bson](decode_bson) decodes a [Bin­ary JSON](http://bsonspec.org/) data format to a json value.

