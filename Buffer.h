#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <mutex>
#include <condition_variable>
#include <deque>
#include <chrono>
#include <stdexcept>

#define PACKET_SIZE 100

/*
 * FiFo Buffer
 */
template <class T> class Buffer
{
    private:
     size_t m_capacity;             // Maximum Capacity
     size_t m_size;                 // Current size
     std::mutex mutex;              // For threadsafing
     std::condition_variable hasCapacity, hasData;  // To notify other threads when capacity/data arrived
    protected:
     std::deque<T> m_buffer;        // Double Ended Queue for fast access to front and back
    public:
     Buffer(size_t capacity=10000); // Constructor with max capacity
     ~Buffer();                     // Destructor
     void push(T elem);                          // Push single element
     void push_batch(T batch[], size_t size);    // Push multiple elements
     T pop();                                    // Pop single element
     void pop_batch(T *batch, size_t size);      // Pop multiple elements
};

#endif