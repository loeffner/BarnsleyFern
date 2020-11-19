#ifndef _FERN_CONSUMER_H_
#define _FERN_CONSUMER_H_

// Required if you do not compile with corresponding libraries (-> shorter cmake file)
// Define before importing CImg.h to disable viewer
#define cimg_display 0 

#include <mutex>
#include "CImg.h"
#include "Consumer.h"
#include "Point.h"
#include "Buffer.h"


#define IMG_PATH "Fern.png"

#define WIDTH 10000 
#define HEIGHT 20000
#define MUTEX_CNT 1000

#define X_MIN -2.2
#define X_MAX 2.7
#define Y_MIN 0.0
#define Y_MAX 10.0

/*
 * Hier findet die tatsaechliche Berechnung der Daten statt
 */
class FernConsumer: public Consumer<Point> {
    public:
     FernConsumer(Buffer<Point>& buffer);
     ~FernConsumer();
     void save();
    private:
     static std::mutex mutexes[MUTEX_CNT];
     static std::mutex save_mutex;
     static cimg_library::CImg<unsigned char> m_image;
     Buffer<Point> m_buffer;    
     static bool saved;
     float m_x_step, m_y_step;
     int m_x_offset, m_y_offset;
     bool consume(Point& datapoint);
     void convert(Point& datapoint);
};

#endif