#include <iostream>
#include "Buffer.h"

template <class T> Buffer<T>::Buffer(size_t capacity) : m_capacity(capacity), m_size(0)
{ }

template <class T> Buffer<T>::~Buffer()
{ }

template <class T> void Buffer<T>::push(T elem)
{
    // Lambda function which checks for free space,
    // true or false determines whether wait() finishes
    std::unique_lock<std::mutex> lock(mutex);
    hasCapacity.wait(lock, [this]
    {
        return m_size < m_capacity;
    });
    m_size++;
    m_buffer.push_back(elem);
    hasData.notify_one();
}

template <class T> void Buffer<T>::push_batch(T elem[], size_t size)
{
        // Lambda function which checks for free space,
        // true or false determines whether wait() finishes
        std::unique_lock<std::mutex> lock(mutex);
        hasCapacity.wait(lock, [this, size]
        {
            return m_size < m_capacity - size;
        });
        for(int i=0; i < size; i++)
        {
            m_size++;
            m_buffer.push_back(elem[i]);
            hasData.notify_one();
        }
}

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