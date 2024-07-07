#pragma once

#include <cstdint>
#include <cstring>
#include <array>
#include <algorithm>
#include <stdexcept>

namespace dict {

    struct Key {
        size_t m_index;
        uint32_t m_key;

        Key(): m_index(0), m_key(0) 
        {}
    };

    struct KeyCompare {
        bool operator()(const Key &a, const Key &b) const { return a.m_key < b.m_key; }
    } keyCompare;

    struct Value {
        uint32_t m_value;
        bool m_flag1;
        bool m_flag2;

        Value(): m_value(0), m_flag1(false), m_flag2(false)
        {}

        Value(uint32_t value): m_value(value), m_flag1(false), m_flag2(false)
        {}
    };

    template<std::size_t N>
    class Dict {
    public:
        using KeyType = std::array<Key,N>;
        using ValueType = std::array<Value,N>;

        Dict(): m_size(0)
        {}

        ~Dict() = default;

        Dict(const Dict &rhs):
            m_size(rhs.m_size),
            m_keys(rhs.m_keys),
            m_values(rhs.m_values)
        {}

        Dict(const uint8_t *buf, size_t size) {
            if (size != sizeof(*this)) {
                throw std::runtime_error("Invalid state data");
            }
            m_size = *(reinterpret_cast<const size_t*>(buf));
            memcpy(m_keys.data(),buf+8,sizeof(m_keys));
            memcpy(m_values.data(),buf+8+sizeof(m_keys), sizeof(m_values));
        }

        Dict& operator=(const Dict &rhs) {
            if (this != &rhs) {
                m_keys = rhs.m_keys;
                m_values = rhs.m_values;
                m_size = rhs.m_size;
            }
            return *this;
        }

        void clear() {
            m_keys.clear();
            m_values.clear();
            m_size = 0;
        }

        std::size_t size() const {
            return m_size;
        }

        std::size_t capacity() const {
            return N;
        }

        const uint8_t *data() const {
            return reinterpret_cast<const uint8_t*>(&m_size);
        }

        size_t container_size() const {
            return sizeof(*this);
        }

        typename KeyType::iterator begin() {
            return m_keys.begin();
        }

        typename KeyType::iterator end() {
            return m_keys.begin() + m_size;
        }

        void insert(const uint32_t &key, const Value &value)
        {
            m_keys[m_size].m_key = key;
            m_keys[m_size].m_index = m_size;
            m_values[m_size] = value;
            m_size++;
            std::sort(&m_keys[0], &m_keys[m_size], keyCompare);
        }

        bool contains(const uint32_t key) {
            auto i = std::find_if(&m_keys[0],&m_keys[m_size],
                [key](Key &elt)
                { return key == elt.m_key; });

            return (i != &m_keys[m_size]);
        }

        Value &at(const uint32_t key) {
            auto i = std::find_if(&m_keys[0],&m_keys[m_size],
                [key](Key &elt)
                { return key == elt.m_key; });
            if (i != &m_keys[m_size]) {
                return m_values.at(i->m_index);
            }
            throw std::out_of_range("Key not found");
        }

        Value &at(const Key &key) {
            auto i = std::find_if(&m_keys[0],&m_keys[m_size],
                [key](Key &elt)
                { return key.m_key == elt.m_key; });
            if (i != &m_keys[m_size]) {
                return m_values.at(i->m_index);
            }
            throw std::out_of_range("Key not found");            
        }

        const Value &at(const uint32_t key) const {
            auto i = std::find_if(&m_keys[0],&m_keys[m_size],
                [key](Key &elt)
                { return key == elt.m_key; });
            if (i != &m_keys[m_size]) {
                return m_values.at(i->m_index);
            }
            throw std::out_of_range("Key not found");
        }

        // Value& operator[](const Key &key) {
            
        // }


    private:
        size_t m_size;
        KeyType m_keys;
        ValueType m_values;

    };


}