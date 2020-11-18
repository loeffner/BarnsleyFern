#include <iostream>
#include "Worker.h"

/*
 * Constructor
 */
Worker::Worker() : m_terminate(false), m_running(false)
{ }

/*
 * Destructor
 */
Worker::~Worker()
{
    stop(false);
};

/*
 * Starten des Threads
 */
void Worker::start()
{
    if(m_running == false)
    {
        // std::cout << "Worker start" << std::endl;
        m_thread = new std::thread(&Worker::work, this);
        m_running = true;
        // std::cout << "Worker started 1" << std::endl;
    }

    // std::cout << "Worker started 2" << std::endl;
}

/*
 * Stoppen des Threads
 */
void Worker::stop(bool force)
{
    m_terminate = force;
    if(m_thread->joinable())
    {
        m_thread->join();
    }
}

/*
 * Schleife die wiederholt step aufruft
 */
void Worker::work()
{
    // std::cout << "Worker work" << std::endl;
    while(m_running && !m_terminate)
    {

        // std::cout << "WOrker step" << std::endl;
        m_running = step();
    }
}

// bool Worker::step()
// {
//     std::cout << "Worker step" << std::endl;
//     return true;
// }