#include "FernProducer.h"
#include <iostream>


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
FernProducer::FernProducer(Buffer<Point>& buffer, size_t max_points, size_t batch_size, size_t id)
 :  Producer(buffer, batch_size), m_max_points(max_points), m_id(id), m_current({0.0, 0.0})
{ 
    dist = std::discrete_distribution<unsigned int>({0.01,   0.85,  0.07,  0.07});
}

/*
 * Destructor
 */
FernProducer::~FernProducer()
{ 
    stop(true);
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
        m_params[rand_set][0] * datapoint.x + m_params[rand_set][1] * datapoint.y + m_params[rand_set][4];
    m_current.y = 
        m_params[rand_set][2] * datapoint.x + m_params[rand_set][3] * datapoint.y + m_params[rand_set][5];
    
    m_nr_points++;

    if(m_nr_points == 1 * m_max_points / 10) std::cout << "Thread " << m_id << " 10%" << std::endl;
    else if(m_nr_points == 2 * m_max_points / 10) std::cout << "Thread " << m_id << " 20%" << std::endl;
    else if(m_nr_points == 3 * m_max_points / 10) std::cout << "Thread " << m_id << " 30%" << std::endl;
    else if(m_nr_points == 4 * m_max_points / 10) std::cout << "Thread " << m_id << " 40%" << std::endl;
    else if(m_nr_points == 5 * m_max_points / 10) std::cout << "Thread " << m_id << " 50%" << std::endl;
    else if(m_nr_points == 6 * m_max_points / 10) std::cout << "Thread " << m_id << " 60%" << std::endl;
    else if(m_nr_points == 7 * m_max_points / 10) std::cout << "Thread " << m_id << " 70%" << std::endl;
    else if(m_nr_points == 8 * m_max_points / 10) std::cout << "Thread " << m_id << " 80%" << std::endl;
    else if(m_nr_points == 9 * m_max_points / 10) std::cout << "Thread " << m_id << " 90%" << std::endl;
    else if(m_nr_points == m_max_points) std::cout << "Thread " << m_id << " finished" << std::endl;
    
    return m_nr_points < m_max_points;
}