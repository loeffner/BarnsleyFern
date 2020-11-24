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
     void push(T elem);                 // Push element
     T pop();                           // Pop element
    protected:
     std::deque<T> m_buffer;            // Double Ended Queue for fast access to front and back
    private:
     size_t m_capacity;                 // Maximum Capacity
     size_t m_size;                     // Current size
     std::mutex mutex;                  // For threadsafing
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


/*
 * Baseclass Worker: Handling of the thread
 */
class Worker {
    public:
     Worker() : m_terminate(false), m_running(false) { };   // Constructor
     virtual ~Worker() { stop(true); }; // Constructor
     void start()                       // Start the thread
     {
        if(m_running == false)
        {
            m_thread = std::thread(&Worker::work, this);
            m_running = true;
            m_terminate = false;
        }
    };
     void stop(bool force)               // Stop the thread (force=true -> hard stop, force=false -> let thread finish on it's own)
     {
         m_terminate = force;
         if(m_thread.joinable())
         {
             m_thread.join();
         }
     };
    protected:                      
     virtual bool step() = 0;       // Pure virtual step function
    private:
     std::thread m_thread;          // The thread
     bool m_terminate;              // Terminate thread?
     bool m_running;                // Thread running?
     void work()                    // Thread loop (repeatedly calling step)
     {
         while(m_running && !m_terminate)
         {
             m_running = step();
         }
         m_running = false;
     };
};



/*
 * Producing and pushing datapoint
 */
template <class T> class Producer : public Worker
{
    public:
     Producer(Buffer<T>& buffer) : Worker(), m_buffer(buffer) { };    // Constructor
     ~Producer() { stop(true); };                                       // Destructor
    protected:
     virtual bool produce(T& datapoint) = 0;    // Generate a new datapoint
     virtual bool step();                       // Generate a new datapoint and push into buffer
    private:
     Buffer<T>& m_buffer;                        // FiFo Buffer
};

/* Generate a new datapoint and push into buffer */
template <class T> bool Producer<T>::step()
{
    bool running = true;
    T datapoint;

    running = produce(datapoint);
    m_buffer.push(datapoint);

    return running;
}


/*
 * Producer produces data and pushes into buffer
 */
template <class T> class Consumer : public Worker
{
    public:
     Consumer(Buffer<T>& buffer) :  Worker(), m_buffer(buffer) { };    // Constructor
     ~Consumer() { stop(true); };                                       // Destructor
    protected:
     virtual bool consume(T& datapoint) = 0;    // Consume datapoints
     virtual bool step();                       // A step consists of producing a datapoint and pushing into the buffer
    private:
     Buffer<T>& m_buffer;           // FiFo Buffer
};

/* Pop and consume datapoints */
template <class T> bool Consumer<T>::step()
{
    bool running;
    T datapoint;

    try
    {
        datapoint = m_buffer.pop();
        running = consume(datapoint);
    }
    catch(const std::runtime_error& e)
    {
        running = false;
    }

    return running;
}


#endif /* _PRO_CON_ */