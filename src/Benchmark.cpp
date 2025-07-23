#include "JobScheduler.hpp"
#include "FixedRetryStrategy.hpp"
#include "Notifier.hpp"
#include "Utils.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

using namespace scheduleit;
using namespace std::chrono;

int main() {
    constexpr int JOB_COUNT = 500000;
    JobScheduler scheduler(std::thread::hardware_concurrency());

    std::shared_ptr<RetryStrategy> retry_strategy = nullptr;

    auto start = high_resolution_clock::now();

    auto noop_task = []() {};
    for (int i = 0; i < JOB_COUNT; ++i) {
        auto job = std::make_shared<Job>(
            "", 
            noop_task,
            retry_strategy,
            std::chrono::milliseconds(0),
            0
        );
        scheduler.submit(job);
    }

    scheduler.start();
    scheduler.waitForIdle();

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    double throughput = static_cast<double>(JOB_COUNT) / duration.count() * 1000.0;
    std::cout << "Submitted and completed " << JOB_COUNT << " jobs in "
              << duration.count() << " ms ("
              << throughput << " jobs/sec)\n";

    scheduler.shutdown();
    return 0;
}