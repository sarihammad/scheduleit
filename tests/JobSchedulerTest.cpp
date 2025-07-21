


#include "JobScheduler.hpp"
#include "Job.hpp"
#include "FixedRetryStrategy.hpp"
#include "Utils.hpp"
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <chrono>
#include <atomic>
#include <thread>

using namespace scheduleit;

TEST_CASE("JobScheduler can start and shutdown cleanly", "[JobScheduler]") {
    JobScheduler scheduler(2);
    scheduler.start();
    utils::sleepForMillis(std::chrono::milliseconds(100));
    scheduler.shutdown();
    SUCCEED("Scheduler started and shut down cleanly");
}

TEST_CASE("JobScheduler submits and executes a job", "[JobScheduler]") {
    JobScheduler scheduler(2);
    scheduler.start();

    auto retry_strategy = std::make_shared<FixedRetryStrategy>(std::chrono::milliseconds(100));
    std::atomic<bool> executed{false};

    auto job = std::make_shared<Job>(
        "test-job",
        [&executed]() { executed = true; },
        retry_strategy
    );

    scheduler.submit(job);
    utils::sleepForMillis(std::chrono::milliseconds(500));
    scheduler.shutdown();

    REQUIRE(executed.load());
}

TEST_CASE("JobScheduler handles shutdown signal correctly", "[JobScheduler]") {
    JobScheduler scheduler(2);
    scheduler.start();

    scheduler.shutdown();
    SUCCEED("Scheduler shut down after shutdown signal without issues");
}

TEST_CASE("JobScheduler handles null job submission safely", "[JobScheduler]") {
    JobScheduler scheduler(2);
    scheduler.start();

    scheduler.submit(nullptr);  // Should not crash
    scheduler.shutdown();
    SUCCEED("Null job submission did not crash");
}