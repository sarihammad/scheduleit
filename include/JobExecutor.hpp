/**
 * @file JobExecutor.hpp
 * @brief Defines the JobExecutor responsible for executing jobs with retry logic.
 */

#pragma once

#include "Job.hpp"
#include "JobQueue.hpp"
#include "Observer.hpp"
#include <memory>
#include <vector>

namespace scheduleit {

/**
 * @class JobExecutor
 * @brief Executes jobs, applying retry strategies and handling rescheduling.
 */
class JobExecutor {
public:
    explicit JobExecutor(std::shared_ptr<JobQueue> job_queue);

    /**
     * @brief Executes the job and handles retry if necessary.
     * @param job The job to be executed.
     */
    void run(std::shared_ptr<Job> job);

    /**
     * @brief Registers an observer to receive job execution notifications.
     * @param observer A shared pointer to the observer instance.
     */
    void registerObserver(std::shared_ptr<Observer> observer);

    /**
     * @brief Returns the number of active jobs currently being executed.
     */
    int getActiveJobCount() const {
        return active_jobs_.load();
    }

private:
    std::shared_ptr<JobQueue> job_queue_;
    std::vector<std::shared_ptr<Observer>> observers_;
    std::atomic<int> active_jobs_{0};
};

}