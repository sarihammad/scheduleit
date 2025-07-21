/**
 * @file ThreadPool.hpp
 * @brief Defines a simple thread pool for executing jobs concurrently.
 */

#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <iostream>

namespace scheduleit {

/**
 * @class ThreadPool
 * @brief A fixed-size thread pool that runs submitted tasks concurrently.
 *
 * Thread-safe and supports graceful shutdown.
 */
class ThreadPool {
public:
    /**
     * @brief Constructs the thread pool with a specified number of worker threads.
     * @param num_threads Number of worker threads to spawn.
     * @param max_queue_size Maximum number of tasks allowed in the queue (0 for unbounded).
     */
    explicit ThreadPool(size_t num_threads, size_t max_queue_size = 0);

    /**
     * @brief Destroys the thread pool and joins all worker threads.
     */
    ~ThreadPool();

    // Delete copy and move constructors and assignment operators
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    /**
     * @brief Submits a task to the thread pool.
     * @tparam F Function type
     * @tparam Args Arguments to the function
     * @param f The function to execute
     * @param args The arguments to the function
     * @return A future that will contain the result of the task.
     */
    template <class F, class... Args>
    auto submit(F&& f, Args&&... args)
        -> std::future<typename std::invoke_result_t<F, Args...>>;

    /**
     * @brief Submits a task with a timeout. If the task doesn't complete in time, it is abandoned.
     * @tparam F Function type
     * @tparam Args Arguments to the function
     * @param f The function to execute
     * @param args The arguments to the function
     * @param timeout Maximum time allowed for the task to run.
     */
    template <class F, class... Args>
    void submitWithTimeout(F&& f, Args&&... args, std::chrono::milliseconds timeout);

    /**
     * @brief Stops the thread pool and discards any remaining tasks.
     */
    void shutdown();

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;

    std::mutex mutex_;
    std::condition_variable condition_;
    std::condition_variable queue_not_full_condition_;
    bool stop_ = false;
    size_t max_queue_size_ = 0;

    void workerLoop();

    template <class Callable>
    void enqueueTask(Callable&& wrapper) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (stop_) {
            throw std::runtime_error("ThreadPool is stopped");
        }
        if (max_queue_size_ > 0) {
            queue_not_full_condition_.wait(lock, [this] {
                return stop_ || tasks_.size() < max_queue_size_;
            });
            if (stop_) {
                throw std::runtime_error("ThreadPool is stopped");
            }
        }
        tasks_.emplace(std::forward<Callable>(wrapper));
        condition_.notify_one();
    }
};

template <class F, class... Args>
auto ThreadPool::submit(F&& f, Args&&... args)
    -> std::future<typename std::invoke_result_t<F, Args...>> {
    using ReturnType = typename std::invoke_result_t<F, Args...>;

    auto task = std::make_shared<std::packaged_task<ReturnType()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<ReturnType> result = task->get_future();
    enqueueTask([task]() { (*task)(); });
    return result;
}

template <class F, class... Args>
void ThreadPool::submitWithTimeout(F&& f, Args&&... args, std::chrono::milliseconds timeout) {
    auto task = std::make_shared<std::packaged_task<void()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::function<void()> wrapper = [task, timeout]() {
        if (timeout.count() > 0) {
            auto future = std::async(std::launch::async, [task]() { (*task)(); });
            if (future.wait_for(timeout) == std::future_status::timeout) {
                std::cerr << "[ThreadPool] Task timed out.\n";
                return;
            }
            future.get(); // propagate exceptions
        } else {
            (*task)(); // no timeout
        }
    };

    enqueueTask(std::move(wrapper));
}

}
