// Copyright Takatoshi Kondo 2023
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This example connects to the specified MQTT broker.
// It then publishes to topic1, topic2, and topic3, and receives the publish results.
// Finally, it disconnects from the broker.
//
// Example:
// ./custom_logger_pub mqtt.redboltz.net 1883

#include <iostream>
#include <string>

#include <boost/asio.hpp>

////////////////////////////////////////////////////////////////////////////////
// Using custom logger BEGIN

// Implement the following codes before any async_mqtt headers include.
// Exception:
//   async_mqtt/util/log_severity.hpp is used for defining custom_log

#include <async_mqtt/util/log_severity.hpp>

// Optional:
// Define custom logger instead of Boost.Log (default)
// undef ASYNC_MQTT_USE_LOG or remove from compiler option
// -DASYNC_MQTT_USE_LOG removes all Boost.Log related code.
// It can compile faster.
#undef ASYNC_MQTT_USE_LOG

// Define simple custom logger
// This is example implementation.
// Just output channel, sevarity, and message body to std::clog
struct custom_log {
    explicit constexpr custom_log(std::string_view chan, async_mqtt::severity_level sev)
    {
        // Setup filter
        if (sev < async_mqtt::severity_level::info) {
            print = false;
            return;
        }
        // Output header
        std::clog << "[" << sev << "]" << "(" << chan << ") ";
    }
    ~custom_log() {
        // Output trailer
        if (print) std::clog << std::endl;
    }

    bool print = true;
};

template <typename T>
inline constexpr custom_log const& operator<<(
    custom_log const& o,
    T const& t
) {
    // Output message body with filter
    if (o.print) std::clog << t;
    return o;
}

// Output additional value. Stringized name(tag) and tagged value.
#define ASYNC_MQTT_ADD_VALUE(name, val) "<" << #name ">{" << val << "} "

// Set ASYNC_MQTT_LOG macro to custom_log.
#define ASYNC_MQTT_LOG(chan, sev) custom_log(chan, async_mqtt::severity_level::sev)

// Using custom logger END
////////////////////////////////////////////////////////////////////////////////

// The following code is the same as cl_cpp17_mqtt_pub.cpp

#include <async_mqtt/all.hpp>


namespace as = boost::asio;
namespace am = async_mqtt;

using client_t = am::client<am::protocol_version::v5, am::protocol::mqtt>;

struct app {
    app(as::any_io_executor exe, std::string_view host, std::string_view port)
        : cli_{exe}
    {
        am::async_underlying_handshake(
            cli_.next_layer(),
            host,
            port,
            [this](auto&&... args) {
                handle_underlying_handshake(
                    std::forward<std::remove_reference_t<decltype(args)>>(args)...
                );
            }
        );
    }

private:
    void handle_underlying_handshake(
        am::error_code ec
    ) {
        std::cout << "underlying_handshake:" << ec.message() << std::endl;
        if (ec) return;
        cli_.async_start(
            true,                // clean_start
            std::uint16_t(0),    // keep_alive
            "",                  // Client Identifier, empty means generated by the broker
            std::nullopt,        // will
            "UserName1",
            "Password1",
            [this](auto&&... args) {
                handle_start_response(std::forward<decltype(args)>(args)...);
            }
        );
    }

    void handle_start_response(
        am::error_code ec,
        std::optional<client_t::connack_packet> connack_opt
    ) {
        std::cout << "start:" << ec.message() << std::endl;
        if (ec) return;
        if (connack_opt) {
            std::cout << *connack_opt << std::endl;
            cli_.async_publish(
                "topic1",
                "payload1",
                am::qos::at_most_once,
                [this](auto&&... args) {
                    handle_publish_response(
                        std::forward<std::remove_reference_t<decltype(args)>>(args)...
                    );
                }
            );
            cli_.async_publish(
                *cli_.acquire_unique_packet_id(), // sync version only works thread safe context
                "topic2",
                "payload2",
                am::qos::at_least_once,
                [this](auto&&... args) {
                    handle_publish_response(
                        std::forward<std::remove_reference_t<decltype(args)>>(args)...
                    );
                }
            );
            cli_.async_publish(
                *cli_.acquire_unique_packet_id(), // sync version only works thread safe context
                "topic3",
                "payload3",
                am::qos::exactly_once,
                [this](auto&&... args) {
                    handle_publish_response(
                        std::forward<std::remove_reference_t<decltype(args)>>(args)...
                    );
                }
            );
        }
    }

    void handle_publish_response(
        am::error_code ec,
        client_t::pubres_type pubres
    ) {
        std::cout << "publish:" << ec.message() << std::endl;
        if (ec) return;
        if (pubres.puback_opt) {
            std::cout << *pubres.puback_opt << std::endl;
        }
        if (pubres.pubrec_opt) {
            std::cout << *pubres.pubrec_opt << std::endl;
        }
        if (pubres.pubcomp_opt) {
            std::cout << *pubres.pubcomp_opt << std::endl;
            cli_.async_disconnect(as::detached);
        }
    }

    client_t cli_;
};

int main(int argc, char* argv[]) {
    am::setup_log(
        am::severity_level::warning,
        true // log colored
    );
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " host port" << std::endl;
        return -1;
    }
    as::io_context ioc;
    app a{ioc.get_executor(), argv[1], argv[2]};
    ioc.run();
}