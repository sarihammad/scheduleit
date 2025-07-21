#include "Job.hpp"
#include "FixedRetryStrategy.hpp"
#include <catch2/catch_test_macros.hpp>
#include <chrono>

using namespace scheduleit;

TEST_CASE("Job tracks attempt count correctly", "[Job]") {
    auto strategy = std::make_shared<FixedRetryStrategy>(std::chrono::milliseconds(100));
    Job job("test-job", [] {}, strategy, std::chrono::milliseconds(0), 3);

    REQUIRE(job.getAttempt() == 0);

    job.incrementAttempt();
    REQUIRE(job.getAttempt() == 1);

    job.incrementAttempt();
    REQUIRE(job.getAttempt() == 2);
}

TEST_CASE("Job shouldRetry respects maxRetries", "[Job]") {
    auto strategy = std::make_shared<FixedRetryStrategy>(std::chrono::milliseconds(100));
    Job job("retry-job", [] {}, strategy, std::chrono::milliseconds(0), 2);

    REQUIRE(job.shouldRetry() == true);
    job.incrementAttempt();
    REQUIRE(job.shouldRetry() == true);
    job.incrementAttempt();
    REQUIRE(job.shouldRetry() == false);
}

TEST_CASE("Job reschedules correctly", "[Job]") {
    auto strategy = std::make_shared<FixedRetryStrategy>(std::chrono::milliseconds(100));
    Job job("delay-job", [] {}, strategy, std::chrono::milliseconds(0), 1);

    auto original_time = job.getScheduledTime();
    job.reschedule(std::chrono::milliseconds(500));

    REQUIRE(job.getScheduledTime() > original_time);
}