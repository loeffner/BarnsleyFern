#include <iostream>
#include "Consumer.h"

/*
 * Constructor
 */
template <class T> Consumer<T>::Consumer(Buffer<T>& buffer)
 : m_buffer(buffer), Worker()
{ }

/*
 * Destructor
 */
template <class T> Consumer<T>::~Consumer(void){};

/* 
 * Warten auf einen Datenpunkt und verarbeiten
 */
template <class T> bool Consumer<T>::step()
{
    bool done;
    T datapoint;
    try
    {
        datapoint = m_buffer.pop();
        done = consume(datapoint);
    }
    catch(const std::runtime_error& e)
    {
        done = false;
    }
    return done;
}
