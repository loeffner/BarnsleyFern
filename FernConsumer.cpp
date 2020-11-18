#include <iostream>
#include "FernConsumer.h"

cimg_library::CImg<unsigned char> FernConsumer::m_image(WIDTH, HEIGHT, 1, 3);
bool FernConsumer::saved = false;

std::mutex FernConsumer::mutexes[MUTEX_CNT];
std::mutex FernConsumer::save_mutex;

/*
 * Constructor
 */
FernConsumer::FernConsumer(Buffer<Point>& buffer) :  Consumer(buffer)
{ 
    m_x_step = (X_MAX - X_MIN) / WIDTH;
    m_x_offset = X_MIN / m_x_step;
    m_y_step = (Y_MAX - Y_MIN) / HEIGHT;
    m_y_offset = Y_MIN / m_y_step;
}

/*
 * Destructor
 */
FernConsumer::~FernConsumer()
{ 
    {
        std::lock_guard<std::mutex> lock(save_mutex);
        if(!saved)
        {
            m_image.save_png(IMG_PATH);
            saved = true;
        }
    }
    stop(false);
}
/* 
 * Generieren der Punkte
 */
bool FernConsumer::consume(Point& datapoint)
{
    unsigned char green[] = { 49, 93, 16 };
    convert(datapoint);
    {
        std::lock_guard<std::mutex> lock(mutexes[(int)(datapoint.x / MUTEX_CNT)]);
        if(m_image((int)datapoint.x, (int)datapoint.y, 0, 1) < 255)
        {
            m_image((int)datapoint.x, (int)datapoint.y, 0, 1)++;
        }
    }
    return true;     
}

void FernConsumer::convert(Point& datapoint)
{
    datapoint.x = datapoint.x / m_x_step - m_x_offset;
    datapoint.y = HEIGHT - datapoint.y / m_y_step - m_y_offset;
}
