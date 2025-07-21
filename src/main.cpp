/**
 * @file main.cpp
 * @brief Entry point for the ScheduleIt job scheduler system.
 */

#include "JobScheduler.hpp"
#include "FixedRetryStrategy.hpp"
#include "Notifier.hpp"
#include "Utils.hpp"
#include "ExponentialBackoffStrategy.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

using namespace scheduleit;

int main() {
    JobScheduler scheduler(4);
    auto notifier = std::make_shared<Notifier>();
    scheduler.getExecutor()->registerObserver(notifier);

    auto retry_strategy = std::make_shared<FixedRetryStrategy>(std::chrono::milliseconds(1000));

    // job 1: succeeds immediately
    auto job1 = std::make_shared<Job>(
        "job-1",
        []() {
            std::cout << "[Job 1] Executing...\n";
        },
        retry_strategy,
        std::chrono::milliseconds(1000),
        2
    );

    // job 2: fails once, then succeeds
    int job2_attempts = 0;
    auto job2 = std::make_shared<Job>(
        "job-2",
        [=]() mutable {
            std::cout << "[Job 2] Executing...\n";
            if (++job2_attempts < 2) {
                throw std::runtime_error("Simulated failure");
            }
        },
        retry_strategy,
        std::chrono::milliseconds(500),
        3
    );

    // job 3: fails a few times, then succeeds (exponential backoff)
    int job3_attempts = 0;
    auto exp_strategy = std::make_shared<ExponentialBackoffStrategy>(
        std::chrono::milliseconds(100), std::chrono::milliseconds(1000));

    auto job3 = std::make_shared<Job>(
        "job-3",
        [=]() mutable {
            std::cout << "[Job 3] Executing (exponential)...\n";
            if (++job3_attempts < 4) {
                throw std::runtime_error("Exp job failed");
            }
        },
        exp_strategy,
        std::chrono::milliseconds(200),
        5
    );

    scheduler.submit(job1);
    scheduler.submit(job2);
    scheduler.submit(job3);

    scheduler.start();
    scheduler.waitForIdle();

    std::cout << "[Main] Shutting down scheduler...\n";
    scheduler.shutdown();

    return 0;
}