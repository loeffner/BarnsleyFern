#include <iostream>
#include "FernConsumer.h"

cimg_library::CImg<unsigned char> FernConsumer::m_image(IMG_WIDTH, IMG_HEIGHT, 1, 3);
bool FernConsumer::saved = false;

std::mutex FernConsumer::mutexes[IMG_MUTEX_CNT];
std::mutex FernConsumer::save_mutex;

/*
 * Constructor
 */
FernConsumer::FernConsumer(Buffer<Point>& buffer, size_t batch_size)
 : Consumer(buffer, batch_size)
{ 
    // Precalculating for conversion from datapoint to pixel-coordinates
    m_x_step = (X_MAX - X_MIN) / IMG_WIDTH;
    m_x_offset = X_MIN / m_x_step;
    m_y_step = (Y_MAX - Y_MIN) / IMG_HEIGHT;
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
            std::cout << "Saving Picture" << std::endl;
            m_image.save_png(IMG_PATH);
            saved = true;
        }
    }
    stop(false);
}

/* Generate points  */
bool FernConsumer::consume(Point& datapoint)
{
    convert(datapoint);
    {
        std::lock_guard<std::mutex> lock(mutexes[(int)(datapoint.y * IMG_MUTEX_CNT / IMG_HEIGHT)]);
        if(m_image((int)datapoint.x, (int)datapoint.y, 0, 1) < 255)
        {
            m_image((int)datapoint.x, (int)datapoint.y, 0, 1)++;
        }
    }
    return true;     
}

/* Convert a datapoint to pixel-coordinates */
void FernConsumer::convert(Point& datapoint)
{
    datapoint.x = datapoint.x / m_x_step - m_x_offset;
    datapoint.y = IMG_HEIGHT - datapoint.y / m_y_step - m_y_offset;
}
