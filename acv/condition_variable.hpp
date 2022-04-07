// Derivative work of Richard Hodges
// (https://cppalliance.org/richard/2020/12/22/RichardsDecemberUpdate.html)

#pragma once

#include <boost/asio.hpp>
#include <mutex>

#include "cpool/types.hpp"

namespace cpool {

class condition_variable {

  public:
    condition_variable(net::any_io_executor exec)
        : timer_(exec) {
        timer_.expires_at(std::chrono::steady_clock::time_point::max());
    }

    template <class Pred>
    [[nodiscard]] net::awaitable<void> async_wait(Pred pred) const;

    [[nodiscard]] net::awaitable<void> notify_one() {
        net::steady_timer notify_timer(co_await net::this_coro::executor);
        notify_timer.expires_from_now(1ms);
        co_await notify_timer.async_wait(use_awaitable);

        timer_.cancel_one();
    }

    [[nodiscard]] net::awaitable<void> notify_all() {
        net::steady_timer notify_timer(co_await net::this_coro::executor);
        notify_timer.expires_from_now(1ms);
        co_await notify_timer.async_wait(use_awaitable);
        timer_.cancel();
    }

  private:
    mutable net::steady_timer timer_;
};

template <class Pred>
net::awaitable<void> condition_variable::async_wait(Pred pred) const {
    error_code ec;
    while (!pred()) {
        co_await timer_.async_wait(net::redirect_error(net::use_awaitable, ec));
    }
}

} // namespace cpool
