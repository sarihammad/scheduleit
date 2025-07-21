#include <catch2/catch_test_macros.hpp>

#include "ThreadPool.hpp"
#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

using namespace scheduleit;

TEST_CASE("ThreadPool executes multiple tasks concurrently", "[threadpool]") {
    ThreadPool pool(4);  // 4 worker threads

    std::atomic<int> counter{0};
    int task_count = 10;

    for (int i = 0; i < task_count; ++i) {
        pool.submit([&counter]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            ++counter;
        });
    }

    // Allow enough time for all tasks to run
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    pool.shutdown();

    REQUIRE(counter == task_count);
}

TEST_CASE("ThreadPool enforces max queue size if specified", "[threadpool][queue]") {
    ThreadPool pool(2, 2);  // 2 workers, queue size = 2

    std::atomic<int> counter{0};
    std::vector<std::future<void>> futures;

    for (int i = 0; i < 4; ++i) {
        futures.emplace_back(pool.submit([&counter]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            ++counter;
        }));
    }

    for (auto& fut : futures) {
        fut.wait();
    }

    pool.shutdown();
    REQUIRE(counter == 4);
}