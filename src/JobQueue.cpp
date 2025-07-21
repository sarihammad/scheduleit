/**
 * @file JobQueue.cpp
 * @brief Implements the JobQueue class for managing scheduled jobs.
 */

#include "JobQueue.hpp"
#include <chrono>
#include "Utils.hpp"
#include <iostream>

namespace scheduleit {

void JobQueue::enqueue(std::shared_ptr<Job> job) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(job));
    }
    cv_.notify_one();
}

std::shared_ptr<Job> JobQueue::dequeueReady() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (!queue_.empty()) {
        auto next_job = queue_.top();
        auto now = utils::now();
        if (now >= next_job->getScheduledTime()) {
            queue_.pop();
            return next_job;
        }
        cv_.wait_until(lock, next_job->getScheduledTime());
    }
    return nullptr;
}

bool JobQueue::empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}

}