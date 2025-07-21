#include "Job.hpp"
#include "Utils.hpp"

namespace scheduleit {

Job::Job(std::string id,
         Task task,
         std::shared_ptr<RetryStrategy> retry_strategy,
         std::chrono::milliseconds delay,
         int max_retries)
    : id_(std::move(id)),
      task_(std::move(task)),
      retry_strategy_(std::move(retry_strategy)),
      max_retries_(max_retries),
      attempt_(0),
      scheduled_time_(utils::now() + delay)
{}

const std::string& Job::getId() const {
    return id_;
}

Job::TimePoint Job::getScheduledTime() const {
    return scheduled_time_;
}

void Job::reschedule(std::chrono::milliseconds delay) {
    scheduled_time_ = utils::now() + delay;
}

void Job::execute() const {
    if (task_) {
        task_();
    }
}

bool Job::shouldRetry() const {
    return attempt_ < max_retries_ && retry_strategy_ && retry_strategy_->shouldRetry(attempt_);
}

int Job::getAttempt() const {
    return attempt_;
}

void Job::incrementAttempt() {
    ++attempt_;
}

int Job::getMaxRetries() const {
    return max_retries_;
}

std::shared_ptr<RetryStrategy> Job::getRetryStrategy() const {
    return retry_strategy_;
}

}