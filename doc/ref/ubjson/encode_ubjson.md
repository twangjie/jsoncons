### jsoncons::ubjson::encode_ubjson

Encodes a C++ data structure to the [Universal Binary JSON Specification (UBJSON)](http://ubjsonspec.org/) data format.

```c++
#include <jsoncons_ext/ubjson/ubjson.hpp>

template<class T>
void encode_ubjson(const T& jval, std::vector<uint8_t>& v); // (1)

template<class T>
void encode_ubjson(const T& jval, std::ostream& os); // (2)

template<class T,class TempAllocator>
void encode_ubjson(temp_allocator_arg_t, const TempAllocator& temp_alloc,
                   const T& jval, std::vector<uint8_t>& v); // (3)

template<class T,class TempAllocator>
void encode_ubjson(temp_allocator_arg_t, const TempAllocator& temp_alloc,
                   const T& jval, std::ostream& os); // (4)
```

(1) Writes a value of type T into a bytes buffer in the UBJSON data format. Type T must be an instantiation of [basic_json](../basic_json.md) 
or support [json_type_traits](../json_type_traits.md). 

(2) Writes a value of type T into a binary stream in the UBJSON data format. Type T must be an instantiation of [basic_json](../basic_json.md) 
or support [json_type_traits](../json_type_traits.md). 

Functions (3)-(4) are the same except `temp_alloc` is used to allocate temporary work areas.

### See also

- [decode_ubjson](decode_ubjson) decodes a [Binary JSON](http://ubjsonspec.org/) data format to a json value.

