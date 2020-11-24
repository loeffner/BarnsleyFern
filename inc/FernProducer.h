#ifndef _FERN_PRODUCER_H_
#define _FERN_PRODUCER_H_

#include <random>
#include <map>
#include <vector>
#include "procon.h"
#include "Point.h"


/*
 * Hier findet die tatsaechliche Berechnung der Daten statt
 */
class FernProducer : public Producer<Point>
{
    public:
     FernProducer(Buffer<Point>& buffer, size_t points, size_t id);
     ~FernProducer();
    private:
     Point m_current;
     std::random_device rand_gen;
     std::discrete_distribution<unsigned int> dist;
     size_t m_nr_points;
     size_t m_max_points;
     size_t m_id;
     static const std::vector<unsigned int> m_prob;
     static const float m_params[4][6];
     static std::mutex cnt_mutex;
     bool produce(Point& datapoint);

};

#endif