/**
 * @file Notifier.cpp
 * @brief Implements job notification logic for success and failure events.
 */

#include "Notifier.hpp"
#include "Utils.hpp"
#include <iostream>

namespace scheduleit {

void Notifier::onJobFailed(const Job& job, int attempt) {
    std::cerr << "[Notifier] Job failed: " << job.getId()
              << " | Attempt: " << attempt
              << " | Time: " << utils::timestampToString(
                   std::chrono::duration_cast<std::chrono::milliseconds>(
                       utils::now().time_since_epoch()))
              << '\n';
}

void Notifier::onJobSuccess(const Job& job) {
    std::cout << "[Notifier] Job succeeded: " << job.getId()
              << " | Time: " << utils::timestampToString(
                   std::chrono::duration_cast<std::chrono::milliseconds>(
                       utils::now().time_since_epoch()))
              << '\n';
}

}