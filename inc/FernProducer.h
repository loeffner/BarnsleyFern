#ifndef _FERN_PRODUCER_H_
#define _FERN_PRODUCER_H_

#include <random>
#include <map>
#include <vector>
#include "CImg.h"
#include "procon.h"
#include "config.h"


/*
 * Hier findet die tatsaechliche Berechnung der Daten statt
 */
class FernProducer : public Producer<Point>
{
    public:
     FernProducer(Buffer<Point>& buffer, size_t max_points, size_t batch_size);
     ~FernProducer();
    private:
     Point m_current;
     std::random_device rand_gen;
     std::discrete_distribution<unsigned int> dist;
     size_t m_nr_points;
     size_t m_max_points;
     static const std::vector<unsigned int> m_prob;
     static const float m_params[4][6];
     static std::mutex cnt_mutex;
     bool produce(Point& datapoint);

};

#endif