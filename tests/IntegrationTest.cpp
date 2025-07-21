#include "JobScheduler.hpp"
#include "FixedRetryStrategy.hpp"
#include "Notifier.hpp"
#include <catch2/catch_test_macros.hpp>
#include <atomic>
#include <chrono>
#include <memory>
#include <thread>

using namespace scheduleit;

TEST_CASE("Integration: JobScheduler executes jobs through queue and executor", "[Integration]") {
    JobScheduler scheduler(2);
    scheduler.start();

    std::atomic<int> counter{0};

    auto retry_strategy = std::make_shared<FixedRetryStrategy>(std::chrono::milliseconds(100));

    auto job = std::make_shared<Job>(
        "integration-job",
        [&counter]() { counter++; },
        retry_strategy,
        std::chrono::milliseconds(100),
        1
    );

    scheduler.submit(job);

    // Give time for execution
    scheduler.waitForIdle();
    scheduler.shutdown();

    REQUIRE(counter == 1);
}