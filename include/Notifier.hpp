/**
 * @file Notifier.hpp
 * @brief Implements a simple observer for job status notifications.
 */

#pragma once

#include "Observer.hpp"
#include "Job.hpp"
#include <memory>

namespace scheduleit {

/**
 * @class Notifier
 * @brief Concrete observer that outputs job events to the console.
 */
class Notifier : public Observer {
public:
    void onJobFailed(const Job& job, int attempt) override;
    void onJobSuccess(const Job& job) override;
};

}