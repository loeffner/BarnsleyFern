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


std::atomic<unsigned int> FernProducer::m_nr_points(0);


/*
 * Constructor
 */
FernProducer::FernProducer(Buffer<Point>& buffer)
 :  Producer(buffer),  m_current({0.0, 0.0})
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
    
    if(m_nr_points%1000000 == 0) std::cout << m_nr_points << std::endl;
    
    return m_nr_points < MAX_POINTS;
}