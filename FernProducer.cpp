#include <iostream>
#include "FernProducer.h"


/* 
 * Static class member: Fern Parameters
 */
const float FernProducer::m_params[4][6] = 
        {{ 0,     0,     0,     0.16,  0,  0   }, 
         { 0.85,  0.04, -0.04,  0.85,  0,  1.6 },
         { 0.2,  -0.26,  0.23,  0.22,  0,  1.6 },
         {-0.15,  0.28,  0.26,  0.24,  0,  0.44}};

const std::vector<unsigned int> FernProducer::m_prob = {1,   85,  7,  7};


/*
 * Constructor
 */
FernProducer::FernProducer(Buffer<Point>& buffer, size_t max_points, size_t batch_size)
 :  Producer(buffer, batch_size), m_max_points(max_points), m_current({0.0, 0.0})
{ 
    dist = std::discrete_distribution<unsigned int>({1,   85,  7,  7});
}

/*
 * Destructor
 */
FernProducer::~FernProducer()
{ 
    stop(false);
}

/* 
 * Generieren der Punkte
 */
bool FernProducer::produce(Point& datapoint)
{   
    unsigned int rand_set = dist(rand_gen); 
    datapoint.x = m_current.x;
    datapoint.y = m_current.y;
   
    m_current.x = 
        m_params[rand_set][0] * m_current.x + m_params[rand_set][1] * m_current.y + m_params[rand_set][4];
    m_current.y = 
        m_params[rand_set][2] * m_current.x + m_params[rand_set][3] * m_current.y + m_params[rand_set][5];
    
    m_nr_points++;
    
    return m_nr_points < m_max_points;
}