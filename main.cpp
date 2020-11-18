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

#define NR_PRODUCER 3
#define NR_CONSUMER 5

int main(void)
{
    time_t start = time(0);

    Buffer<Point> buffer(10);


    // All our Producers
    std::vector<FernProducer*> producers;
    for (int i=0; i < NR_PRODUCER; ++i)
    {
        FernProducer *p = new FernProducer(buffer, i);
        producers.push_back(p);
    }

    // Start producing
    for (auto &p : producers) p->start();

    // All our Consumers
    std::vector<FernConsumer*> consumers;
    for (int i=0; i < NR_CONSUMER; ++i)
    {
        FernConsumer *c = new FernConsumer(buffer);
        consumers.push_back(c);
    }

    // Start producing
    for (auto &c : consumers) c->start();



    // Clean up producers
    for (auto &p : producers) delete p;

    // Clean up consumers
    for (auto &c : consumers) delete c;
    
    time_t end = time(0);
    std::cout << "Runtime: "  << difftime(end, start) * 1000.0 << std::endl;

return 0;
}