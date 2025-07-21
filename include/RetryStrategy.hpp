/**
 * @file RetryStrategy.hpp
 * @brief Defines the abstract interface for retry strategies.
 */

#pragma once

#include <chrono>

namespace scheduleit {

/**
 * @class RetryStrategy
 * @brief Strategy interface for retrying failed jobs.
 *
 * Allows flexible retry logic such as fixed intervals or exponential backoff.
 */
class RetryStrategy {
public:
    virtual ~RetryStrategy() = default;

    /**
     * @brief Determines if a job should be retried based on the current attempt.
     * @param attempt The current attempt count (0-based)
     * @return True if the job should be retried, false otherwise.
     */
    virtual bool shouldRetry(int attempt) const = 0;

    /**
     * @brief Computes the delay before the next retry attempt.
     * @param attempt The current attempt count (0-based)
     * @return Duration to wait before retrying.
     */
    virtual std::chrono::milliseconds getBackoffDelay(int attempt) const = 0;
};

}