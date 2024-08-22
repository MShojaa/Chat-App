#include "pch.h"
#include "framework.h"

#include "Flow.h"

namespace msh {

    /// <summary>
    /// Locks the mutex to write into the value and just notify to one listener
    /// </summary>
    /// <param name="value"> Adds this value to the buffer (queue) </param>
    void Flow::push_async(const std::string &value) {
        std::lock_guard<std::mutex> lock(mutex_);
        this->push(value);
        data_available_cv.notify_one();
    }

    /// <summary>
    /// Locks the mutex to read from the buffer
    /// Waits for a signal (notify)
    /// Removes (pops) the first element (FIFO - queue) after copying to a temp variable for return of the function
    /// Unlocks the mutex for furthur use (write again)
    /// </summary>
    /// <returns> nullptr if reading is completed and std::string as data if there is some data to read </returns>
    std::optional<std::string> Flow::pop_async() {

        std::unique_lock<std::mutex> lock(mutex_);
        //if (!is_read_completed) {
            data_available_cv.wait(lock, [this] { 
                return !this->empty() || is_read_completed;
            });
        //}

        if (is_read_completed && this->empty()) {
            is_read_completed = false;
            return std::nullopt;
        }

        std::string chunk = this->front();
        this->pop();
        lock.unlock();

        return chunk;
    }

    /// <summary>
    /// Checks if there is still some data left to write in the buffer
    /// </summary>
    /// <returns></returns>
    /*bool Flow::is_finished() {
        return is_read_completed && this->empty();
    }*/

    /// <summary>
    /// Completed data write to the buffer and waits for data to pop-out of buffer (read data - pop_async())
    /// </summary>
    void Flow::finished() {
        is_read_completed = true;
    }

    std::string Flow::last_element() {
        return this->back();
    }

    /// <summary>
    /// Initializes field variable
    /// </summary>
    Flow::Flow() {
        is_read_completed = false;
    }
}