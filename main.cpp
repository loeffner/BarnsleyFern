#include "Buffer.h"
#include "Buffer.cpp"
#include "Producer.h"
#include "Producer.cpp"
#include "Consumer.h"
#include "Consumer.cpp"
#include "FernProducer.h"
#include "FernProducer.cpp"
#include "FernConsumer.h"
#include "FernConsumer.cpp"

#define NR_PRODUCER 6
#define NR_CONSUMER 4

int main(void)
{
    // FiFo Buffer
    Buffer<Point> buffer(10000);

    // All our Producers
    std::vector<FernProducer*> producers;
    for (int i=0; i < NR_PRODUCER; ++i)
    {
        FernProducer *p = new FernProducer(buffer);
        producers.push_back(p);
    }

    // All our Consumers
    std::vector<FernConsumer*> consumers;
    for (int i=0; i < NR_CONSUMER; ++i)
    {
        FernConsumer *c = new FernConsumer(buffer);
        consumers.push_back(c);
    }


    // Start producing
    for (auto &p : producers) p->start();

    // Start consuming
    for (auto &c : consumers) c->start();


    // Clean up producers
    for (auto &p : producers) delete p;

    // Clean up consumers
    for (auto &c : consumers) delete c;

    return 0;
}