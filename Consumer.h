#ifndef _CONSUMER_H_
#define _CONSUMER_H_

#include <stdexcept>
#include "Worker.h"
#include "Buffer.h"


/*
 * Producer produces data and pushes into buffer
 */
template <class T> class Consumer : public Worker
{
    public:
     Consumer(Buffer<T>& buffer, size_t batch_size);    // Constructor
     ~Consumer();                                       // Destructor
    protected:
     virtual bool consume(T& datapoint) = 0;    // Consume datapoints
     virtual bool step();                       // A step consists of producing a datapoint and pushing into the buffer
    private:
     Buffer<T>& m_buffer;           // FiFo Buffer
     size_t m_batch_size;           // Nr of points to consume at a time
};

#endif