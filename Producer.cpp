#include <iostream>
#include "Producer.h"

/* Constructor */
template <class T> Producer<T>::Producer(Buffer<T>& buffer)
 : m_buffer(buffer), Worker()
{ }

/* Destructor */
template <class T> Producer<T>::~Producer() 
{ }

/* Erzeugen eines neuen Datenpunktes mit produce() und Speichern im Buffer */
template <class T> bool Producer<T>::step()
{
    bool running = true;
    unsigned int i;
    T datapoint[PACKET_SIZE];
    for(i=0; i < PACKET_SIZE && running; i++)
    {
        running = produce(datapoint[i]);
    }
    m_buffer.push_batch(datapoint, i);
    return running;
}
