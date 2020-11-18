#ifndef _FERN_PRODUCER_H_
#define _FERN_PRODUCER_H_

#include <random>
#include <map>
#include <atomic>
#include "Producer.h"
#include "Point.h"

#define MAX_POINTS 1000000000

/*
 * Hier findet die tatsaechliche Berechnung der Daten statt
 */
class FernProducer : public Producer<Point>
{
    public:
     FernProducer(Buffer<Point>& buffer, unsigned int id);
     ~FernProducer();
    protected:
     bool produce(Point& datapoint);
     Point m_current;
    private:
     unsigned int m_id;
     std::random_device rand_gen;
     std::discrete_distribution<> dist;
     static std::atomic<unsigned int> m_nr_points;
     static const float m_prob[4];
     static const float m_params[4][6];

};

#endif