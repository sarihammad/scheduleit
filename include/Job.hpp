/**
 * @file Job.hpp
 * @brief Defines the Job class representing a schedulable unit of work.
 */

#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <atomic>
#include "RetryStrategy.hpp"

namespace scheduleit {

/**
 * @class Job
 * @brief Represents a unit of work to be executed by the scheduler.
 *
 * Each job includes a unique identifier, an executable task (as a lambda or function),
 * retry configuration, scheduling delay, and current attempt state.
 */
class Job {
public:
    using TimePoint = std::chrono::system_clock::time_point;
    using Task = std::function<void()>;

    Job(std::string id,
        Task task,
        std::shared_ptr<RetryStrategy> retry_strategy,
        std::chrono::milliseconds delay = std::chrono::milliseconds(0),
        int max_retries = 3);

    const std::string& getId() const;
    TimePoint getScheduledTime() const;
    void reschedule(std::chrono::milliseconds delay);
    void execute() const;
    bool shouldRetry() const;
    int getAttempt() const;
    void incrementAttempt();
    int getMaxRetries() const;
    std::shared_ptr<RetryStrategy> getRetryStrategy() const;

private:
    std::string id_;
    Task task_;
    std::shared_ptr<RetryStrategy> retry_strategy_;
    int max_retries_;
    std::atomic<int> attempt_;
    TimePoint scheduled_time_;

    bool is_shutdown_signal_ = false;
};

}