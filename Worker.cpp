#include "Worker.h"
#include <iostream>

/* Constructor */
Worker::Worker() : m_terminate(false), m_running(false)
{ }

/* Destructor */
Worker::~Worker()
{
    stop(false);
};

/* Starten des Threads */
void Worker::start()
{
    if(m_running == false)
    {
        m_thread = new std::thread(&Worker::work, this);
        m_running = true;
    }

}

/* Stoppen des Threads */
void Worker::stop(bool force)
{
    m_terminate = force;
    if(m_thread->joinable())
    {
        m_thread->join();
    }
}

/* Schleife die wiederholt step aufruft */
void Worker::work()
{
    while(m_running && !m_terminate)
    {

        m_running = step();
    }
}
