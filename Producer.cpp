#include "Producer.h"

/* Constructor */
template <class T> Producer<T>::Producer(Buffer<T>& buffer, size_t batch_size)
 : Worker(), m_buffer(buffer), m_batch_size(batch_size)
{ }

/* Destructor */
template <class T> Producer<T>::~Producer() 
{ }

/* Generate a new datapoint and push into buffer */
template <class T> bool Producer<T>::step()
{
    bool running = true;
    unsigned int i;
    T datapoint[m_batch_size];
    for(i=0; i < m_batch_size && running; i++)
    {
        running = produce(datapoint[i]);
    }
    m_buffer.push_batch(datapoint, i);
    return running;
}
