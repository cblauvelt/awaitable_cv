#pragma once

#include <functional>
#include <tuple>

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/experimental/as_tuple.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/write.hpp>

#include <batteries/errors/error.hpp>

#ifdef ACV_TRACE_LOGGING
#include <fmt/format.h>
#include <iostream>
#define TRACE_LOG(prefix, ...)                                                 \
    (std::cout << fmt::format("[{0}] {1}", prefix, fmt::format(__VA_ARGS__))   \
               << std::endl);
#else
#define TRACE_LOG(prefix, ...)
#endif

namespace cpool {
namespace net = boost::asio;
using namespace boost::asio::experimental::awaitable_operators;
using namespace std::chrono_literals;

using error = batteries::errors::error;
using tcp = net::ip::tcp;
using error_code = boost::system::error_code;
using system_error = boost::system::system_error;
using time_point = typename std::chrono::steady_clock::time_point;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
using boost::asio::experimental::as_tuple;
using std::chrono::milliseconds;
using std::chrono::seconds;

} // namespace cpool
