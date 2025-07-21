/**
 * @file FixedRetryStrategy.cpp
 * @brief Implements the FixedRetryStrategy class.
 */

#include "FixedRetryStrategy.hpp"

namespace scheduleit {

FixedRetryStrategy::FixedRetryStrategy(std::chrono::milliseconds delay)
    : delay_(delay) {}

bool FixedRetryStrategy::shouldRetry(int attempt) const {
    return true; 
}

std::chrono::milliseconds FixedRetryStrategy::getBackoffDelay(int /*attempt*/) const {
    return delay_;
}

}
