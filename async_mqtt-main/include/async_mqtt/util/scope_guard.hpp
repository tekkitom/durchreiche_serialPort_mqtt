// Copyright Takatoshi Kondo 2022
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if !defined(ASYNC_MQTT_UTIL_SCOPE_GUARD_HPP)
#define ASYNC_MQTT_UTIL_SCOPE_GUARD_HPP

#include <memory>
#include <utility>

namespace async_mqtt {

namespace detail {
template <typename Proc>
class unique_sg {
public:
    explicit unique_sg(Proc proc)
        :proc_{std::move(proc)}
    {}
    unique_sg(unique_sg const&) = delete;
    unique_sg(unique_sg&& other)
        :proc_{other.proc_} {
        other.moved_ = true;
    }
    ~unique_sg() {
        if (!moved_) std::move(proc_)();
    }

private:
    Proc proc_;
    bool moved_ = false;
};

} // namespace detail

template <typename Proc>
inline detail::unique_sg<Proc> unique_scope_guard(Proc&& proc) {
    return detail::unique_sg<Proc>{std::forward<Proc>(proc)};
}

template <typename Proc>
inline auto shared_scope_guard(Proc&& proc) {
    auto deleter = [proc = std::forward<Proc>(proc)](void*) mutable { std::move(proc)(); };
    return std::shared_ptr<void>(nullptr, std::move(deleter));
}

} // namespace async_mqtt

#endif // ASYNC_MQTT_UTIL_SCOPE_GUARD_HPP
