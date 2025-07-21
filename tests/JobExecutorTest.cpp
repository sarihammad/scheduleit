#include "JobExecutor.hpp"
#include "JobQueue.hpp"
#include "FixedRetryStrategy.hpp"
#include "Job.hpp"
#include "Observer.hpp"
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <atomic>

using namespace scheduleit;

namespace {
class CountingObserver : public Observer {
public:
    std::atomic<int> success_count{0};
    std::atomic<int> failure_count{0};

    void onJobFailed(const Job& job, int attempt) override {
        ++failure_count;
    }

    void onJobSuccess(const Job& job) override {
        ++success_count;
    }
};
}

TEST_CASE("JobExecutor retries a failing job up to maxRetries", "[JobExecutor]") {
    auto job_queue = std::make_shared<JobQueue>();
    JobExecutor executor(job_queue);

    auto observer = std::make_shared<CountingObserver>();
    executor.registerObserver(observer);

    auto fail_count = std::make_shared<std::atomic<int>>(0);

    auto strategy = std::make_shared<FixedRetryStrategy>(std::chrono::milliseconds(1));
    auto job = std::make_shared<Job>(
        "retry-job",
        [fail_count]() {
            (*fail_count)++;
            throw std::runtime_error("fail");
        },
        strategy,
        std::chrono::milliseconds(0),
        3  
    );

    executor.run(job);

    REQUIRE(observer->success_count == 0);
}