#pragma once
/**
 * @file JobQueue.hpp
 * @brief Defines a thread-safe priority queue for scheduled jobs.
 */

#include "Job.hpp"
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>

namespace scheduleit {

/**
 * @class JobQueue
 * @brief Thread-safe priority queue for scheduling jobs based on execution time.
 */
class JobQueue {
public:
    /**
     * @brief Adds a job to the queue.
     * @param job The job to be enqueued.
     */
    void enqueue(std::shared_ptr<Job> job);

    /**
     * @brief Waits until the next job is ready or the queue is empty and retrieves it.
     * @return The job ready to be executed, or nullptr if empty.
     */
    std::shared_ptr<Job> dequeueReady();

    /**
     * @brief Checks if the queue is empty.
     */
    bool empty() const;

    /**
     * @brief Increments the internal pending job count (used for synchronization).
     */
    void incrementPending() {
        ++pending_count_;
    }

    void decrementPending() {
        --pending_count_;
    }

    int getPendingCount() const {
        return pending_count_;
    }

    /**
     * @brief Checks if there are no pending jobs.
     */
    bool isIdle() const;

private:
    struct CompareJob {
        bool operator()(const std::shared_ptr<Job>& lhs, const std::shared_ptr<Job>& rhs) const {
            return lhs->getScheduledTime() > rhs->getScheduledTime(); // min-heap
        }
    };

    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::priority_queue<std::shared_ptr<Job>, std::vector<std::shared_ptr<Job>>, CompareJob> queue_;
    std::atomic<int> pending_jobs_{0};
    std::atomic<int> pending_count_ = 0;
};

}