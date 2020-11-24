#include "procon.h"
#include "FernProducer.h"
#include "FernConsumer.h"
#include "config.h"


int main(void)
{
    // FiFo Buffer
    Buffer<Point> buffer(BUFFER_SIZE);

    // All our Producers
    std::vector<FernProducer*> producers;
    for (int i=0; i < NR_PRODUCER; ++i)
    {
        FernProducer *p = new FernProducer(buffer, NR_POINTS, i);
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


    // Wait for producer threads to finish
    for (auto &p : producers) p->stop(false);

    // Wait for consumer threads to finish
    for (auto &c : consumers) c->stop(false);


    // Clean up producers
    for (auto &p : producers) delete p;

    // Clean up consumers
    for (auto &c : consumers) delete c;


    return 0;
}