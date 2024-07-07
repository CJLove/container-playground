#include "gtest/gtest.h"

#include "flatdict.h"

using namespace dict;

TEST(container_test, basicTests) {
    Dict<10> dict;
    EXPECT_EQ(10,dict.capacity());
    EXPECT_EQ(0,dict.size());
    EXPECT_EQ(sizeof(dict), dict.container_size());

    EXPECT_EQ((unsigned char*)&dict, dict.data());
}

TEST(container_test, insertion) {
    Dict<5> dict;
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
    } catch (...) {
        // Expected exception
    }
}

TEST(container_test, updates) {
    Dict<5> dict;
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
    Dict<10> dict;

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