// Derivative work of Richard Hodges
// (https://cppalliance.org/richard/2020/12/22/RichardsDecemberUpdate.html)

#pragma once

#include <boost/asio.hpp>
#include <mutex>

#include "acv/types.hpp"

namespace acv {

class condition_variable {

  public:
    condition_variable(net::any_io_executor exec)
        : timer_(exec) {
        timer_.expires_at(std::chrono::steady_clock::time_point::max());
    }

    template <class Pred>
    [[nodiscard]] net::awaitable<void> async_wait(Pred pred) const;

    void notify_one() { timer_.cancel_one(); }

    void notify_all() { timer_.cancel(); }

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

} // namespace acv
