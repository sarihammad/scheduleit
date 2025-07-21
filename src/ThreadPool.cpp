/**
 * @file ThreadPool.cpp
 * @brief Implements the ThreadPool class for concurrent task execution.
 */

#include "ThreadPool.hpp"
#include <iostream>
#include <future>
#include <chrono>

namespace scheduleit {

ThreadPool::ThreadPool(size_t num_threads, size_t max_queue_size)
    : max_queue_size_(max_queue_size) {
    for (size_t i = 0; i < num_threads; ++i) {
        workers_.emplace_back([this]() { workerLoop(); });
    }
}

ThreadPool::~ThreadPool() {
    shutdown();
}

void ThreadPool::shutdown() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        stop_ = true;
    }
    condition_.notify_all();
    queue_not_full_condition_.notify_all();
    for (auto& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void ThreadPool::workerLoop() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(mutex_);
            condition_.wait(lock, [this]() { return stop_ || !tasks_.empty(); });

            if (stop_ && tasks_.empty()) {
                return;
            }

            task = std::move(tasks_.front());
            tasks_.pop();
            queue_not_full_condition_.notify_one();
        }

        if (task) {
            try {
                task();
            } catch (const std::exception& e) {
                std::cerr << "[ThreadPool] Task threw exception: " << e.what() << '\n';
            } catch (...) {
                std::cerr << "[ThreadPool] Task threw unknown exception.\n";
            }
        }
    }
}

}