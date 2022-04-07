#define ACV_TRACE_LOGGING

#include <cpool/cpool.hpp>
#include <fmt/format.h>
#include <iostream>
#include <optional>

using std::cout;
using std::endl;
using namespace cpool;

const std::string ENV_HOST = "CLIENT_HOST";
const std::string DEFAULT_HOST = "host.docker.internal";
const std::string ENV_PORT = "CLIENT_PORT";
const std::string DEFAULT_PORT = "80";

std::optional<std::string> get_env_var(std::string const& key) {
    char* val = getenv(key.c_str());
    return (val == NULL) ? std::nullopt : std::optional(std::string(val));
}

awaitable<batteries::errors::error>
on_connection_state_change(cpool::tcp_connection* conn,
                           const cpool::client_connection_state state) {
    switch (state) {
    case cpool::client_connection_state::disconnected:
        cout << fmt::format("disconnected from {0}:{1}", conn->host(),
                            conn->port())
             << endl;
        break;

    case cpool::client_connection_state::resolving:
        cout << fmt::format("resolving {0}", conn->host()) << endl;
        break;

    case cpool::client_connection_state::connecting:
        cout << fmt::format("connecting to {0}:{1}", conn->host(), conn->port())
             << endl;
        break;

    case cpool::client_connection_state::connected:
        cout << fmt::format("connected to {0}:{1}", conn->host(), conn->port())
             << endl;
        break;

    case cpool::client_connection_state::disconnecting:
        cout << fmt::format("disconnecting from {0}:{1}", conn->host(),
                            conn->port())
             << endl;
        break;

    default:
        cout << fmt::format("unknown client_connection_state: {0}", (int)state)
             << endl;
    }

    co_return batteries::errors::error();
}

awaitable<void> run_client(boost::asio::io_context& ctx) {
    auto executor = co_await net::this_coro::executor;
    auto host = get_env_var(ENV_HOST).value_or(DEFAULT_HOST);
    auto portString = get_env_var(ENV_PORT).value_or(DEFAULT_PORT);
    uint16_t port = std::stoi(portString);

    cpool::tcp_connection connection(executor, host, port);
    connection.set_state_change_handler(on_connection_state_change);

    auto error = co_await connection.async_connect();
    cout << "Connection status: (" << error.value() << "):" << error.message()
         << endl;
    if (error) {
        co_return;
    }

    error = co_await connection.async_disconnect();
    cout << "Disconnection status: (" << error.value()
         << "):" << error.message() << endl;

    ctx.stop();
}

int main(void) {
    net::io_context ctx(1);
    co_spawn(ctx, run_client(std::ref(ctx)), detached);
    ctx.run();
    return 0;
}