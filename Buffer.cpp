#include "Buffer.h"

/* Constructor */
template <class T> Buffer<T>::Buffer(size_t capacity)
 : m_capacity(capacity), m_size(0)
{ }

/* Destructor */
template <class T> Buffer<T>::~Buffer()
{ }

/* Push single element */
template <class T> void Buffer<T>::push(T elem)
{
    std::unique_lock<std::mutex> lock(mutex);
    hasCapacity.wait(lock, [this]
    {
        return m_size < m_capacity;
    });
    m_size++;
    m_buffer.push_back(elem);
    hasData.notify_one();
}

/* Push batch of elements */
template <class T> void Buffer<T>::push_batch(T batch[], size_t size)
{
    std::unique_lock<std::mutex> lock(mutex);
    hasCapacity.wait(lock, [this, size]
    {
        return m_size + size <= m_capacity;
    });
    for(int i=0; i < size; i++)
    {
        m_size++;
        m_buffer.push_back(batch[i]);
    }
    hasData.notify_one();
}

/* Pop single element */
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

/* Pop multiple elements */
template <class T> void Buffer<T>::pop_batch(T *batch, size_t size)
{
    std::unique_lock<std::mutex> lock(mutex);
    if(
        hasData.wait_for(lock, std::chrono::seconds(1),
        [this, size] 
        {
            return m_size >= size;
        }))
    {
        for(int i=0; i < size; i++)
        {
            m_size--;
            batch[i] = m_buffer.front();
            m_buffer.pop_front();
        }
        hasCapacity.notify_one();
    }
    else
    {
        throw std::runtime_error("No Data incoming.");
    }
}