/**
 * @file ExponentialBackoffStrategy.hpp
 * @brief Defines a retry strategy with exponential backoff between attempts.
 */

#pragma once

#include "RetryStrategy.hpp"
#include <chrono>
#include <algorithm>

namespace scheduleit {

/**
 * @class ExponentialBackoffStrategy
 * @brief Implements exponential backoff retry strategy with optional max delay cap.
 *
 * This strategy increases the delay between retries exponentially, starting from a configurable base delay.
 * The delay is capped at a maximum value to prevent excessively long waits.
 */
class ExponentialBackoffStrategy : public RetryStrategy {
public:
    /**
     * @brief Constructor.
     * @param base_delay Initial delay for the first retry.
     * @param max_delay Maximum delay to cap the exponential growth.
     */
    ExponentialBackoffStrategy(std::chrono::milliseconds base_delay,
                               std::chrono::milliseconds max_delay);

    bool shouldRetry(int attempt) const override;
    std::chrono::milliseconds getBackoffDelay(int attempt) const override;

private:
    std::chrono::milliseconds base_delay_;
    std::chrono::milliseconds max_delay_;
};

}