#include "Consumer.h"
#include <iostream>

/* Constructor */
template <class T> Consumer<T>::Consumer(Buffer<T>& buffer, size_t batch_size)
 :  Worker(), m_buffer(buffer), m_batch_size(batch_size)
{ }

/* Destructor */
template <class T> Consumer<T>::~Consumer(void){};

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
