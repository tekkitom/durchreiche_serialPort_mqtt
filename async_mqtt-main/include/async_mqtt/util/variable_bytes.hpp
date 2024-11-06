// Copyright Takatoshi Kondo 2022
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if !defined(ASYNC_MQTT_UTIL_VARIABLE_BYTES_HPP)
#define ASYNC_MQTT_UTIL_VARIABLE_BYTES_HPP

#include <cstdint>
#include <optional>

#include <async_mqtt/util/static_vector.hpp>
#include <async_mqtt/util/detail/is_iterator.hpp>

namespace async_mqtt {

inline static_vector<char, 4>
val_to_variable_bytes(std::uint32_t val) {
    static_vector<char, 4> bytes;
    if (val > 0xfffffff) return bytes;
    while (val > 127) {
        bytes.push_back(static_cast<char>((val & 0b01111111) | 0b10000000));
        val >>= 7;
    }
    bytes.push_back(val & 0b01111111);
    return bytes;
}


template <typename It, typename End>
constexpr
std::enable_if_t<
    detail::is_input_iterator<It>::value &&
    detail::is_input_iterator<End>::value,
    std::optional<std::uint32_t>
>
variable_bytes_to_val(It& it, End e) {
    std::size_t val = 0;
    std::size_t mul = 1;
    for (; it != e; ++it) {
        if (mul == 128 * 128 * 128 * 128) {
            ++it;
            return std::nullopt;
        }
        val += (*it & 0b01111111) * mul;
        mul *= 128;
        if (!(*it & 0b10000000)) {
            ++it;
            return std::uint32_t(val);
        }
    }
    return std::nullopt;
}

} // namespace async_mqtt

#endif // ASYNC_MQTT_UTIL_VARIABLE_BYTES_HPP
