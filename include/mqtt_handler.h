#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <iostream>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <async_mqtt5.hpp>

namespace asio = boost::asio;

namespace async_mqtt5 {

asio::awaitable<void> client_receiver(asio::io_context& ioc);

} // namespace async_mqtt5

#endif // MQTT_HANDLER_H