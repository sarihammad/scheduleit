/**
 * @file JobExecutor.cpp
 * @brief Implements the JobExecutor class for job execution with retry logic.
 */

#include "JobExecutor.hpp"
#include "Utils.hpp"
#include <thread>
#include <iostream>
#include <atomic>

namespace scheduleit {

JobExecutor::JobExecutor(std::shared_ptr<JobQueue> job_queue)
    : job_queue_(std::move(job_queue)), active_jobs_(0) {}

void JobExecutor::registerObserver(std::shared_ptr<Observer> observer) {
    if (observer) {
        observers_.emplace_back(std::move(observer));
    }
}

void JobExecutor::run(std::shared_ptr<Job> job) {
    active_jobs_++;
    try {
        job->execute();
        // notify observers of success
        for (const auto& obs : observers_) {
            if (obs) obs->onJobSuccess(*job);
        }
    } catch (const std::exception& e) {
        // notify observers of failure
        for (const auto& obs : observers_) {
            if (obs) obs->onJobFailed(*job, job->getAttempt());
        }
        // retry logic
        if (job->shouldRetry()) {
            job->incrementAttempt();
            auto strategy = job->getRetryStrategy();
            if (strategy) {
                auto delay = strategy->getBackoffDelay(job->getAttempt());
                job->reschedule(delay);
                utils::sleepForMillis(delay);  // simulate backoff before retry enqueue
                job_queue_->incrementPending();
                job_queue_->enqueue(std::move(job));
            }
        }
    }
    job_queue_->decrementPending();
    active_jobs_--;
}

}