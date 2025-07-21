/**
 * @file Utils.hpp
 * @brief Provides utility functions for time handling and formatting.
 */

#pragma once

#include <chrono>
#include <string>
#include <thread>
#include <ctime>

namespace scheduleit {
namespace utils {

/**
 * @brief Returns the current steady clock time point.
 */
inline std::chrono::system_clock::time_point now() {
    return std::chrono::system_clock::now();
}

/**
 * @brief Converts milliseconds since epoch to a human-readable timestamp string.
 * @param ms Duration since epoch.
 * @return Timestamp string.
 */
inline std::string timestampToString(std::chrono::milliseconds ms) {
    auto time = std::chrono::system_clock::time_point(ms);
    std::time_t time_t_val = std::chrono::system_clock::to_time_t(time);
    char buf[100];
    std::strftime(buf, sizeof(buf), "%F %T", std::localtime(&time_t_val));
    return std::string(buf);
}

/**
 * @brief Sleeps for the specified duration in milliseconds.
 * @param ms Duration to sleep.
 */
inline void sleepForMillis(std::chrono::milliseconds ms) {
    std::this_thread::sleep_for(ms);
}

}

}