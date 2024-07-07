#include "flatdict.h"
#include <iostream>
#include <cstring>

using namespace dict;

/**
 * @brief Notional Value for example code
 *
 */
struct Value
{
    uint32_t m_value;
    bool m_flag1;
    bool m_flag2;

    Value() : m_value(0), m_flag1(false), m_flag2(false)
    {
    }

    Value(uint32_t value) : m_value(value), m_flag1(false), m_flag2(false)
    {
    }
};

int main(int , char**)
{
    Dict<Value,10> dict;

    std::cout << "Key size " << sizeof(Key) << " Value size " << sizeof(Value) << "\n";
    std::cout << "Dict<10> size " << sizeof(dict) << "\n";

    std::cout << "dict.size() " << dict.size() << " dict.capacity() " << dict.capacity() << "\n";

    dict.insert(333,Value(44));
    dict.insert(222,Value(10));
    dict.insert(111,Value(456));

    std::cout << "dict.size() " << dict.size() << " dict.capacity() " << dict.capacity() << "\n";

    try {
        auto &v = dict.at(333);
        std::cout << "dict[" << 333 << "] = " << v.m_value << "\n";
    } catch (std::exception &e) {
        std::cout << "Caught exception " << e.what() << "\n";
    }

    try {
        auto &v = dict.at(222);
        std::cout << "dict[" << 222 << "] = " << v.m_value << "\n";
    } catch (std::exception &e) {
        std::cout << "Caught exception " << e.what() << "\n";
    }

    try {
        auto &v = dict.at(111);
        std::cout << "dict[" << 111 << "] = " << v.m_value << "\n";
    } catch (std::exception &e) {
        std::cout << "Caught exception " << e.what() << "\n";
    }

    try {
        auto &v = dict.at(123);
        std::cout << "dict[" << 123 << "] = " << v.m_value << "\n";
    } catch (std::exception &e) {
        std::cout << "Caught exception " << e.what() << "\n";
    }

    std::cout << "dict.contains(111) " << dict.contains(111) << "\n";
    std::cout << "dict.contains(222) " << dict.contains(222) << "\n";
    std::cout << "dict.contains(333) " << dict.contains(333) << "\n";
    std::cout << "dict.contains(123) " << dict.contains(123) << "\n";

    for (const auto &elt: dict) {
        std::cout << "dict[" << elt.m_key << "] = " << dict.at(elt).m_value << "\n";
    }

    auto *buffer = dict.data();
    size_t size = dict.container_size();

    Dict<Value,10> dict2(buffer,size);
    std::cout << "dict2.size() " << dict2.size() << " dict2.capacity() " << dict2.capacity() << "\n";

    for (const auto &elt: dict2) {
        std::cout << "dict2[" << elt.m_key << "] = " << dict.at(elt).m_value << "\n";
    }

}
