#ifndef _WORKER_H_
#define _WORKER_H_

#include <thread>

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

#endif