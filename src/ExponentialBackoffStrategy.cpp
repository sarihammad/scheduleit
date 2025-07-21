/**
 * @file ExponentialBackoffStrategy.cpp
 * @brief Implements the ExponentialBackoffStrategy class.
 */

#include "ExponentialBackoffStrategy.hpp"

namespace scheduleit {

ExponentialBackoffStrategy::ExponentialBackoffStrategy(
    std::chrono::milliseconds base_delay,
    std::chrono::milliseconds max_delay)
    : base_delay_(base_delay), max_delay_(max_delay) {}

bool ExponentialBackoffStrategy::shouldRetry(int attempt) const {
    return true;
}

std::chrono::milliseconds ExponentialBackoffStrategy::getBackoffDelay(int attempt) const {
    auto delay = base_delay_.count() * (1LL << attempt);  // exponential backoff
    return std::chrono::milliseconds(std::min(delay, max_delay_.count()));
}

}