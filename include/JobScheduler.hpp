/**
 * @file JobScheduler.hpp
 * @brief Declares the JobScheduler responsible for orchestrating job execution.
 */

#pragma once

#include "Job.hpp"
#include "JobQueue.hpp"
#include "JobExecutor.hpp"
#include "ThreadPool.hpp"
#include <memory>
#include <atomic>
#include <thread>
#include <vector>

namespace scheduleit {

/**
 * @class JobScheduler
 * @brief Manages job submission, scheduling, and dispatching to the thread pool.
 */
class JobScheduler {
public:
    /**
     * @brief Constructs the scheduler with specified thread pool size.
     * @param num_workers Number of worker threads.
     */
    explicit JobScheduler(size_t num_workers);

    /**
     * @brief Destructor. Shuts down the scheduler.
     */
    ~JobScheduler();

    /**
     * @brief Starts the internal dispatcher thread.
     */
    void start();

    /**
     * @brief Submits a job to be scheduled.
     * @param job The job to submit.
     */
    void submit(std::shared_ptr<Job> job);

    /**
     * @brief Gracefully shuts down the scheduler.
     */
    void shutdown();

    /**
     * @brief Returns the internal job executor instance.
     */
    JobExecutor* getExecutor() const;

    /**
     * @brief Blocks until the job queue is empty and all tasks are processed.
     */
    void waitForIdle();

private:
    std::shared_ptr<JobQueue> job_queue_;
    std::unique_ptr<ThreadPool> thread_pool_;
    std::unique_ptr<JobExecutor> executor_;
    std::thread dispatcher_thread_;
    std::atomic<bool> running_;
};

}