#include "Consumer.h"
#include <iostream>

/* Constructor */
template <class T> Consumer<T>::Consumer(Buffer<T>& buffer)
 : m_buffer(buffer), Worker()
{ }

/* Destructor */
template <class T> Consumer<T>::~Consumer(void){};

/* Warten auf einen Datenpunkt und verarbeiten */
template <class T> bool Consumer<T>::step()
{
    bool running;
    T batch[PACKET_SIZE];
    try
    {
        m_buffer.pop_batch(batch, PACKET_SIZE);
        for(int i=0; i < PACKET_SIZE; i++)
            running = consume(batch[i]);
    }
    catch(const std::runtime_error& e)
    {
        running = false;
    }
    return running;
}
