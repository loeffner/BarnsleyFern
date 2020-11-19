#ifndef _FERN_PRODUCER_H_
#define _FERN_PRODUCER_H_

#include <random>
#include <map>
#include <atomic>
#include <vector>
#include "Producer.h"
#include "Point.h"

#define MAX_POINTS 100000000

/*
 * Hier findet die tatsaechliche Berechnung der Daten statt
 */
class FernProducer : public Producer<Point>
{
    public:
     FernProducer(Buffer<Point>& buffer);
     ~FernProducer();
    private:
     Point m_current;
     std::random_device rand_gen;
     std::discrete_distribution<unsigned int> dist;
     static std::atomic<unsigned int> m_nr_points;
     static const std::vector<unsigned int> m_prob;
     static const float m_params[4][6];
     static std::mutex cnt_mutex;
     bool produce(Point& datapoint);

};

#endif