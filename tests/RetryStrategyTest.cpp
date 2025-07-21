#include "ExponentialBackoffStrategy.hpp"
#include "FixedRetryStrategy.hpp"
#include <catch2/catch_test_macros.hpp>
#include <chrono>

using namespace scheduleit;

TEST_CASE("FixedRetryStrategy returns constant delay", "[RetryStrategy]") {
    auto strategy = FixedRetryStrategy(std::chrono::milliseconds(500));

    REQUIRE(strategy.shouldRetry(0));
    REQUIRE(strategy.shouldRetry(10));
    REQUIRE(strategy.getBackoffDelay(0) == std::chrono::milliseconds(500));
    REQUIRE(strategy.getBackoffDelay(5) == std::chrono::milliseconds(500));
}

TEST_CASE("ExponentialBackoffStrategy computes exponential delays", "[RetryStrategy]") {
    auto base = std::chrono::milliseconds(100);
    auto max = std::chrono::milliseconds(1000);
    auto strategy = ExponentialBackoffStrategy(base, max);

    REQUIRE(strategy.shouldRetry(0));
    REQUIRE(strategy.getBackoffDelay(0) == std::chrono::milliseconds(100));
    REQUIRE(strategy.getBackoffDelay(1) == std::chrono::milliseconds(200));
    REQUIRE(strategy.getBackoffDelay(2) == std::chrono::milliseconds(400));
    REQUIRE(strategy.getBackoffDelay(3) == std::chrono::milliseconds(800));
    REQUIRE(strategy.getBackoffDelay(4) == std::chrono::milliseconds(1000)); // capped
    REQUIRE(strategy.getBackoffDelay(5) == std::chrono::milliseconds(1000)); // capped
}