/**
 * @file FixedRetryStrategy.hpp
 * @brief Defines a retry strategy with a fixed delay between attempts.
 */

#pragma once

#include "RetryStrategy.hpp"
#include <chrono>

namespace scheduleit {

/**
 * @class FixedRetryStrategy
 * @brief Implements a simple retry strategy with a constant backoff delay.
 */
class FixedRetryStrategy : public RetryStrategy {
public:
    /**
     * @brief Constructor.
     * @param delay The fixed delay between retries.
     */
    explicit FixedRetryStrategy(std::chrono::milliseconds delay);

    bool shouldRetry(int attempt) const override;
    std::chrono::milliseconds getBackoffDelay(int attempt) const override;

private:
    std::chrono::milliseconds delay_;
};

}