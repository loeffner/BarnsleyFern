#include <iostream>
#include "Producer.h"

/*
 * Constructor
 */
template <class T> Producer<T>::Producer(Buffer<T>& buffer)
 : m_buffer(buffer), Worker()
{ }

/* 
 * Erzeugen eines neuen Datenpunktes mit produce() und Speichern im Buffer
 */
template <class T> bool Producer<T>::step()
{
    bool done;
    T datapoint[PACKET_SIZE];
    for(int i=0; i < PACKET_SIZE; i++)
    {
        done = produce(datapoint[i]);
    }
    m_buffer.push_batch(datapoint, PACKET_SIZE);
    return done;
}

/*
 * Destructor
 */
template <class T> Producer<T>::~Producer() 
{ }
