/**
 * @file Observer.hpp
 * @brief Declares the Observer interface for job event notifications.
 */

#pragma once

#include "Job.hpp"

namespace scheduleit {

/**
 * @class Observer
 * @brief Interface for observing job execution results.
 */
class Observer {
public:
    virtual ~Observer() = default;

    /**
     * @brief Called when a job fails.
     * @param job The job that failed.
     * @param attempt The attempt count.
     */
    virtual void onJobFailed(const Job& job, int attempt) = 0;

    /**
     * @brief Called when a job succeeds.
     * @param job The job that succeeded.
     */
    virtual void onJobSuccess(const Job& job) = 0;
};

}