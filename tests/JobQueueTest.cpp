#include "JobQueue.hpp"
#include "Job.hpp"
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <thread>

using namespace scheduleit;

TEST_CASE("JobQueue orders jobs by scheduled time", "[JobQueue]") {
    JobQueue queue;

    auto job1 = std::make_shared<Job>("job1", [] {}, nullptr, std::chrono::milliseconds(100));
    auto job2 = std::make_shared<Job>("job2", [] {}, nullptr, std::chrono::milliseconds(10));

    queue.enqueue(job1);
    queue.enqueue(job2);

    auto dequeued = queue.dequeueReady();
    REQUIRE(dequeued->getId() == "job2");

    dequeued = queue.dequeueReady();
    REQUIRE(dequeued->getId() == "job1");
}

TEST_CASE("JobQueue blocks until job is ready", "[JobQueue]") {
    JobQueue queue;

    auto job = std::make_shared<Job>("delayed", [] {}, nullptr, std::chrono::milliseconds(100));
    queue.enqueue(job);

    auto start = std::chrono::system_clock::now();
    auto dequeued = queue.dequeueReady();
    auto end = std::chrono::system_clock::now();

    REQUIRE(dequeued->getId() == "delayed");
    REQUIRE(end - start >= std::chrono::milliseconds(90));
}