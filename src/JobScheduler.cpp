/**
 * @file JobScheduler.cpp
 * @brief Implements the JobScheduler class responsible for coordinating job execution.
 */

#include "JobScheduler.hpp"
#include <iostream>

namespace scheduleit {

JobScheduler::JobScheduler(size_t num_workers)
    : job_queue_(std::make_shared<JobQueue>()),
      thread_pool_(std::make_unique<ThreadPool>(num_workers)),
      executor_(std::make_unique<JobExecutor>(job_queue_)),
      running_(false) {}

JobScheduler::~JobScheduler() {
    shutdown();
}

void JobScheduler::start() {
    running_ = true;
    dispatcher_thread_ = std::thread([this]() {
        while (running_ || !job_queue_->empty() || job_queue_->getPendingCount() > 0 || executor_->getActiveJobCount() > 0) {
            auto job = job_queue_->dequeueReady();
            if (!job) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            thread_pool_->submit([this, job]() {
                executor_->run(job);
            });
        }
    });
}

void JobScheduler::submit(std::shared_ptr<Job> job) {
    if (!job) {
        std::cerr << "[JobScheduler] Warning: Attempted to submit null job. Ignoring.\n";
        return;
    }
    job_queue_->enqueue(std::move(job));
}

void JobScheduler::shutdown() {
    running_ = false;
    if (dispatcher_thread_.joinable()) {
        dispatcher_thread_.join();
    }
    thread_pool_->shutdown();
}

JobExecutor* JobScheduler::getExecutor() const {
    return executor_.get();
}

void JobScheduler::waitForIdle() {
    const int idle_threshold = 5;
    int stable_count = 0;

    while (stable_count < idle_threshold) {
        if (!job_queue_->empty() ||
            job_queue_->getPendingCount() > 0 ||
            executor_->getActiveJobCount() > 0) {
            stable_count = 0;
        } else {
            ++stable_count;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

}