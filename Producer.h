#ifndef _PRODUCER_H_
#define _PRODUCER_H_

#include "Worker.h"
#include "Buffer.h"

/*
 * Producing and pushing datapoint
 */
template <class T> class Producer : public Worker
{
    public:
     Producer(Buffer<T>& buffer, size_t batch_size);    // Constructor
     ~Producer();                                       // Destructor
    protected:
     virtual bool produce(T& datapoint) = 0;    // Generate a new datapoint
     virtual bool step();                       // Generate a new datapoint and push into buffer
    private:
     Buffer<T>& m_buffer;                        // FiFo Buffer
     size_t m_batch_size;
};

#endif