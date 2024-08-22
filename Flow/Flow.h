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
    class Flow : std::queue<std::string>
    {
    public:
        /// <summary>
        /// Locks the mutex to write into the value and just notify to one listener
        /// </summary>
        /// <param name="value"> Adds this value to the buffer (queue) </param>
        void push_async(const std::string &value);

        /// <summary>
        /// Locks the mutex to read from the buffer
        /// Waits for a signal (notify)
        /// Removes (pops) the first element (FIFO - queue) after copying to a temp variable for return of the function
        /// Unlocks the mutex for furthur use (write again)
        /// </summary>
        /// <returns> nullptr if reading is completed and std::string as data if there is some data to read </returns>
        std::optional<std::string> pop_async();

        /// <summary>
        /// Completed data write to the buffer and waits for data to pop-out of buffer (read data - pop_async())
        /// </summary>
        void finished();

        /// <summary>
        /// Returns the last element pushed to the queue
        /// </summary>
        /// <returns> the last element pushed to the queue </returns>
        std::string last_element();

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