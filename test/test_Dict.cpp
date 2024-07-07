#include "gtest/gtest.h"

#include "flatdict.h"

using namespace dict;

/**
 * @brief Notional Value class for unit tests (all POD types)
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

TEST(container_test, basicTests) {
    Dict<Value,10> dict;
    EXPECT_EQ(10,dict.capacity());
    EXPECT_EQ(0,dict.size());
    EXPECT_EQ(sizeof(dict), dict.container_size());

    EXPECT_EQ((unsigned char*)&dict, dict.data());
}

TEST(container_test, insertion) {
    Dict<Value,5> dict;
    // insert() with key present/not present
    EXPECT_TRUE(dict.insert(111,456));
    EXPECT_FALSE(dict.insert(111,666));

    // insert() up to capacity
    EXPECT_TRUE(dict.insert(555,222));
    EXPECT_TRUE(dict.insert(444,333));
    EXPECT_TRUE(dict.insert(333,444));
    EXPECT_TRUE(dict.insert(222,555));

    EXPECT_EQ(5, dict.size());

    // insert() fails if full
    EXPECT_FALSE(dict.insert(666,111));

    try {
        auto v  = dict.at(444);
        EXPECT_EQ(333,v.m_value);
    } catch (...) {
        // Unexpected exception
        FAIL();
    }

    try {
        auto v  = dict.at(123);
        EXPECT_EQ(v.m_value, v.m_value);
        FAIL();
    } catch (std::exception &e) {
        // Expected exception
        std::cout << "Caught expected " << e.what() << "\n";
    }
}

TEST(container_test, updates) {
    Dict<Value,5> dict;
    // set() with key not present
    EXPECT_FALSE(dict.set(444,123));

    EXPECT_TRUE(dict.insert(444,123));
    EXPECT_TRUE(dict.set(444,555));

    try {
        auto v  = dict.at(444);
        EXPECT_EQ(555,v.m_value);
    } catch (...) {
        // Unexpected exception
        FAIL();
    }
}

TEST(container_test, ranges) {
    Dict<Value,10> dict;

    EXPECT_TRUE(dict.insert(111,111));
    EXPECT_TRUE(dict.insert(555,555));
    EXPECT_TRUE(dict.insert(444,444));
    EXPECT_TRUE(dict.insert(333,333));
    EXPECT_TRUE(dict.insert(222,222));

    uint32_t count = 0;
    for (const auto &elt: dict) {
        count++;
        auto v = dict.at(elt);
        EXPECT_EQ(111*count, elt.m_key);
        EXPECT_EQ(111*count, v.m_value);
    }
    EXPECT_EQ(5,count);

    EXPECT_TRUE(dict.insert(666,666));

    count = 0;
    for (const auto &elt: dict) {
        count++;
        auto v = dict.at(elt);
        EXPECT_EQ(111*count, elt.m_key);
        EXPECT_EQ(111*count, v.m_value);
    }
    EXPECT_EQ(6,count);

}

TEST(container_test, serialize) {
    Dict<Value,3> dict;
    EXPECT_TRUE(dict.insert(111,456));
    EXPECT_TRUE(dict.insert(222,789));

    const uint32_t *dataPtr = reinterpret_cast<const uint32_t*>(dict.data());

    // container size: 8 + 8*3 + 8*3 = 56
    EXPECT_EQ(56,dict.container_size());

    // container m_size: 
    EXPECT_EQ(2,dataPtr[0]);

    // container m_keys[0]
    EXPECT_EQ(0,dataPtr[2]);
    EXPECT_EQ(111,dataPtr[3]);

    // container m_keys[1]
    EXPECT_EQ(1,dataPtr[4]);    
    EXPECT_EQ(222,dataPtr[5]);

    // container m_keys[2]
    EXPECT_EQ(0,dataPtr[6]);
    EXPECT_EQ(0,dataPtr[7]);

    // container m_values[0]
    EXPECT_EQ(456,dataPtr[8]);

    // container m_values[1]
    EXPECT_EQ(789,dataPtr[10]);

}

TEST(container_test, deserialize) {
    uint32_t data[] {
        2,
        0,
        0, 111,
        1, 222,
        0, 0,
        111, 0,
        222, 0,
        0, 0
    };

    try
    {
        Dict<Value, 3> dict(reinterpret_cast<uint8_t *>(&data), sizeof(data));

        EXPECT_EQ(2, dict.size());
        EXPECT_EQ(3, dict.capacity());
        EXPECT_EQ(sizeof(data), dict.container_size());

        uint32_t count = 0;
        for (const auto &elt : dict)
        {
            count++;
            auto v = dict.at(elt);
            EXPECT_EQ(111 * count, elt.m_key);
            EXPECT_EQ(111 * count, v.m_value);
        }
        EXPECT_EQ(2, count);
    }
    catch (...)
    {
        // Unexpected exception
        FAIL();
    }
}

TEST(container_test, deserializeFailSize) {
    uint32_t data[] {
        2,
        0,
        0, 111,
        1, 222,
        0, 0,
        111, 0,
        222, 0,
    };

    try
    {
        Dict<Value, 3> dict(reinterpret_cast<uint8_t *>(&data), sizeof(data));

        FAIL();
    }
    catch (std::exception &e)
    {
        // Expected exception
        std::cout << "Caught expected " << e.what() << "\n";
    }
}

TEST(container_test, deserializeFailContent) {
    uint32_t data[] {
        4,
        0,
        0, 111,
        1, 222,
        0, 0,
        111, 0,
        222, 0,
        0, 0
    };

    try
    {
        Dict<Value, 3> dict(reinterpret_cast<uint8_t *>(&data), sizeof(data));

        FAIL();
    }
    catch (std::exception &e)
    {
        // Expected exception
        std::cout << "Caught expected " << e.what() << "\n";
    }
}

TEST(container_test, deserializeFailIndex) {
    uint32_t data[] {
        2,
        0,
        4, 111, // Invalid index value > container capacity
        1, 222,
        0, 0,
        111, 0,
        222, 0,
        0, 0
    };

    try
    {
        Dict<Value, 3> dict(reinterpret_cast<uint8_t *>(&data), sizeof(data));

        FAIL();
    }
    catch (std::exception &e)
    {
        // Expected exception
        std::cout << "Caught expected " << e.what() << "\n";
    }
}