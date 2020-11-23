#ifndef _PRO_CON_
#define _PRO_CON_

#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <chrono>
#include <stdexcept>


/*
 * FiFo Buffer
 */
template <class T> class Buffer
{    public:
     Buffer(size_t capacity) : m_capacity(capacity), m_size(0) { };       // Constructor with max capacity
     ~Buffer() { };                     // Destructor
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

/* Push single element */
template <class T> void Buffer<T>::push(T elem)
{
    std::unique_lock<std::mutex> lock(mutex);
    hasCapacity.wait(lock, [this]
    {
        return m_size < m_capacity;
    });
    m_size++;
    m_buffer.push_back(elem);
    hasData.notify_one();
}

/* Push batch of elements */
template <class T> void Buffer<T>::push_batch(T batch[], size_t size)
{
    std::unique_lock<std::mutex> lock(mutex);
    hasCapacity.wait(lock, [this, size]
    {
        return m_size + size <= m_capacity;
    });
    for(int i=0; i < size; i++)
    {
        m_size++;
        m_buffer.push_back(batch[i]);
    }
    hasData.notify_one();
}

/* Pop single element */
template <class T> T Buffer<T>::pop()
{
    std::unique_lock<std::mutex> lock(mutex);
    T elem;
    if(
        hasData.wait_for(lock, std::chrono::seconds(1),
        [this] 
        {
            return m_size > 0;
        }))
    {
        m_size--;
        elem = m_buffer.front();
        m_buffer.pop_front();
        hasCapacity.notify_one();
        return elem;
    }
    else
    {
        throw std::runtime_error("No Data incoming.");
    }
}

/* Pop multiple elements */
template <class T> void Buffer<T>::pop_batch(T *batch, size_t size)
{
    std::unique_lock<std::mutex> lock(mutex);
    if(
        hasData.wait_for(lock, std::chrono::seconds(1),
        [this, size] 
        {
            return m_size >= size;
        }))
    {
        for(int i=0; i < size; i++)
        {
            m_size--;
            batch[i] = m_buffer.front();
            m_buffer.pop_front();
        }
        hasCapacity.notify_one();
    }
    else
    {
        throw std::runtime_error("No Data incoming.");
    }
}



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
     std::thread m_thread;         // The thread
     bool m_terminate;              // Terminate thread?
     bool m_running;                // Thread running?
     void work();                   // Thread loop (repeatedly calling step)
};


/*
 * Producing and pushing datapoint
 */
template <class T> class Producer : public Worker
{
    public:
     Producer(Buffer<T>& buffer, size_t batch_size) : Worker(), m_buffer(buffer), m_batch_size(batch_size) { };    // Constructor
     ~Producer() { stop(true); };                                       // Destructor
    protected:
     virtual bool produce(T& datapoint) = 0;    // Generate a new datapoint
     virtual bool step();                       // Generate a new datapoint and push into buffer
    private:
     Buffer<T>& m_buffer;                        // FiFo Buffer
     size_t m_batch_size;
};

/* Generate a new datapoint and push into buffer */
template <class T> bool Producer<T>::step()
{
    int i;
    bool running = true;
    T datapoint[m_batch_size];
    for(i=0; i < m_batch_size && running; i++)
    {
        running = produce(datapoint[i]);
    }
    m_buffer.push_batch(datapoint, i);
    return running;
}


/*
 * Producer produces data and pushes into buffer
 */
template <class T> class Consumer : public Worker
{
    public:
     Consumer(Buffer<T>& buffer, size_t batch_size) :  Worker(), m_buffer(buffer), m_batch_size(batch_size) { };    // Constructor
     ~Consumer() { stop(true); };                                       // Destructor
    protected:
     virtual bool consume(T& datapoint) = 0;    // Consume datapoints
     virtual bool step();                       // A step consists of producing a datapoint and pushing into the buffer
    private:
     Buffer<T>& m_buffer;           // FiFo Buffer
     size_t m_batch_size;           // Nr of points to consume at a time
};

/* Pop and consume datapoints */
template <class T> bool Consumer<T>::step()
{
    bool running;
    T batch[m_batch_size];
    try
    {
        m_buffer.pop_batch(batch, m_batch_size);
        for(int i=0; i < m_batch_size; i++)
            running = consume(batch[i]);
    }
    catch(const std::runtime_error& e)
    {
        running = false;
    }
    return running;
}


#endif /* _PRO_CON_ */