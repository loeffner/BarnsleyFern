#include "FernConsumer.h"
#include <iostream>

cimg_library::CImg<unsigned char> FernConsumer::m_image(IMG_WIDTH, IMG_HEIGHT, 1, 3);
bool FernConsumer::saved = false;

std::mutex FernConsumer::mutexes[IMG_MUTEX_CNT];
std::mutex FernConsumer::save_mutex;

/*
 * Constructor
 */
FernConsumer::FernConsumer(Buffer<Point>& buffer, size_t batch_size)
 : Consumer(buffer, batch_size)
{ }

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
    stop(true);
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
    datapoint.x = datapoint.x * IMG_WIDTH / (X_MAX - X_MIN) - X_MIN * IMG_WIDTH / (X_MAX - X_MIN) ;
    datapoint.y = IMG_HEIGHT  - datapoint.y * IMG_HEIGHT / (Y_MAX - Y_MIN) - Y_MIN * IMG_HEIGHT / (Y_MAX - Y_MIN) ;
}
