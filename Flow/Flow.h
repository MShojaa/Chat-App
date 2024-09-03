#pragma once

#include "pch.h"

/*
* How to implement: (example)
* 
* // Creates an async buffer
* msh::Flow buffer;
* 
* // Creates a container to consume results
* std::string content = "Some content";
* std::optional<std::string> result;
* 
* // Writes data to buffer asynchronously
* std::thread write_to_buffer_thread([&buffer, &content]() {
* 	buffer.push_async(content);
* 	});
* 
* // Consumes data from buffer
* std::thread read_and_pop_from_buffer_thread([&buffer, &result]() {
* result = buffer.pop_async();
* 	});
* 
* // Waits for threads to join
* write_to_buffer_thread.join();
* read_and_pop_from_buffer_thread.join();
* 
* // Handles error
* if (!result.has_value())
* 	return EXIT_FAILURE;
*/

namespace msh {
    template <typename T = std::string>
    class Flow : std::queue<T>
    {
    public:
        /// <summary>
        /// Locks the mutex to write into the value and just notify to one listener
        /// </summary>
        /// <param name="value"> Adds this value to the buffer (queue) </param>
        void push_async(const T &value);

        /// <summary>
        /// Locks the mutex to read from the buffer
        /// Waits for a signal (notify)
        /// Removes (pops) the first element (FIFO - queue) after copying to a temp variable for return of the function
        /// Unlocks the mutex for furthur use (write again)
        /// </summary>
        /// <returns> nullptr if reading is completed and std::string as data if there is some data to read </returns>
        std::optional<T> pop_async();

        /// <summary>
        /// Completed data write to the buffer and waits for data to pop-out of buffer (read data - pop_async())
        /// </summary>
        void finished();

        /// <summary>
        /// Returns the last element pushed to the queue
        /// </summary>
        /// <returns> the last element pushed to the queue </returns>
        T last_element();

        T first_element();

        bool wait_for_element(const T& element);

        bool dont_wait_for_element(const T& element);

        bool wait_to_fill();

        bool wait_to_empty();

        void clear_all();

        /// <summary>
        /// Initializes Field is_read_completed to false
        /// </summary>
        Flow();

    private:
        /// <summary>
        /// Stores mutex to lock read/write
        /// </summary>
        std::mutex mutex_;

        /// <summary>
        /// To check if data is available or not (waiting for signal)
        /// </summary>
        std::condition_variable data_available_cv;

        /// <summary>
        /// Stores boolean to check if writing to buffer is done or not
        /// </summary>
        bool is_read_completed;

        /// <summary>
        /// Checks if 
        /// </summary>
        /// <returns></returns>
        //bool is_finished();
    };
}

// Definitions
namespace msh {

    /// <summary>
    /// Locks the mutex to write into the value and just notify to one listener
    /// </summary>
    /// <param name="value"> Adds this value to the buffer (queue) </param>
    template <typename T>
    void Flow<T>::push_async(const T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        this->push(value);
        data_available_cv.notify_all();
    }

    /// <summary>
    /// Locks the mutex to read from the buffer
    /// Waits for a signal (notify)
    /// Removes (pops) the first element (FIFO - queue) after copying to a temp variable for return of the function
    /// Unlocks the mutex for furthur use (write again)
    /// </summary>
    /// <returns> nullptr if reading is completed and std::string as data if there is some data to read </returns>
    template <typename T>
    std::optional<T> Flow<T>::pop_async() {

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

        T chunk = this->front();
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
    template <typename T>
    void Flow<T>::finished() {
        is_read_completed = true;
    }

    template <typename T>
    T Flow<T>::last_element() {
        //return this->back();
        std::unique_lock<std::mutex> lock(mutex_);
        data_available_cv.wait(lock, [this] {
            return !this->empty();
        });

        T result = this->back();
        lock.unlock();
        return result;
    }

    template <typename T>
    T Flow<T>::first_element() {
        std::unique_lock<std::mutex> lock(mutex_);
        data_available_cv.wait(lock, [this] {
            return !this->empty();
            });

        T result = this->front();
        lock.unlock();
        return result;
    }

    template <typename T>
    bool Flow<T>::wait_for_element(const T& element) {
        std::unique_lock<std::mutex> lock(mutex_);
        data_available_cv.wait(lock, [this, &element] {
            return (!this->empty() && this->front() == element);
            });

        lock.unlock();
        return true;
    }

    template <typename T>
    bool Flow<T>::dont_wait_for_element(const T& element) {
        std::unique_lock<std::mutex> lock(mutex_);
        data_available_cv.wait(lock, [this, &element] {
            return (!this->empty() && this->front() != element);
            });

        lock.unlock();
        return true;
    }

    template <typename T>
    bool Flow<T>::wait_to_fill() {
        std::unique_lock<std::mutex> lock(mutex_);
        data_available_cv.wait(lock, [this] {
            return !this->empty();
            });

        lock.unlock();
        return true;
    }

    template <typename T>
    bool Flow<T>::wait_to_empty() {
        std::unique_lock<std::mutex> lock(mutex_);
        data_available_cv.wait(lock, [this] {
            return this->empty();
            });

        lock.unlock();
        return true;
    }

    template <typename T>
    void Flow<T>::clear_all() {
        if (!this->empty()) {
            std::lock_guard<std::mutex> lock(mutex_);
            auto temp = std::queue<T>();
            this->swap(temp);
            data_available_cv.notify_all();
        }
    }

    /// <summary>
    /// Initializes field variable
    /// </summary>
    template <typename T>
    Flow<T>::Flow() {
        is_read_completed = false;
    }
}