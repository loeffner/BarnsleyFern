#ifndef _PRO_CON_
#define _PRO_CON_

#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <chrono>
#include <stdexcept>

/* 
 * Darstellung eines (Daten)Punktes
 */
typedef struct datapoint_t {
    float x, y;
} Point;


/*
 * FiFo Buffer
 */
template <class T> class Buffer
{    public:
     Buffer(size_t capacity);       // Constructor with max capacity
     ~Buffer();                     // Destructor
     void push(T elem);                          // Push single element
     void push_batch(T batch[], size_t size);    // Push multiple elements
     T pop();                                    // Pop single element
     void pop_batch(T *batch, size_t size);      // Pop multiple elements
    protected:
     std::deque<T> m_buffer;        // Double Ended Queue for fast access to front and back
    private:
     size_t m_capacity;             // Maximum Capacity
     size_t m_size;                 // Current size
     std::mutex mutex;              // For threadsafing
     std::condition_variable hasCapacity, hasData;  // To notify other threads when capacity/data arrived

};


/*
 * Baseclass Worker: Handling of the thread
 */
class Worker {
    public:
     Worker();                      // Constructor
     virtual ~Worker();             // Constructor
     void start();                  // Start the thread
     void stop(bool force);         // Stop the thread (force=true -> hard stop, force=false -> let thread finish on it's own)
    protected:                      
     virtual bool step() = 0;       // Pure virtual step function
    private:
     std::thread *m_thread;         // The thread
     bool m_terminate;              // Terminate thread?
     bool m_running;                // Thread running?
     void work();                   // Thread loop (repeatedly calling step)
};


/*
 * Producer produces data and pushes into buffer
 */
template <class T> class Consumer : public Worker
{
    public:
     Consumer(Buffer<T>& buffer, size_t batch_size);    // Constructor
     ~Consumer();                                       // Destructor
    protected:
     virtual bool consume(T& datapoint) = 0;    // Consume datapoints
     virtual bool step();                       // A step consists of producing a datapoint and pushing into the buffer
    private:
     Buffer<T>& m_buffer;           // FiFo Buffer
     size_t m_batch_size;           // Nr of points to consume at a time
};


/*
 * Producing and pushing datapoint
 */
template <class T> class Producer : public Worker
{
    public:
     Producer(Buffer<T>& buffer, size_t batch_size);    // Constructor
     ~Producer();                                       // Destructor
    protected:
     virtual bool produce(T& datapoint) = 0;    // Generate a new datapoint
     virtual bool step();                       // Generate a new datapoint and push into buffer
    private:
     Buffer<T>& m_buffer;                        // FiFo Buffer
     size_t m_batch_size;
};

#endif /* _PRO_CON_ */