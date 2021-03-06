#ifndef _FERN_CONSUMER_H_
#define _FERN_CONSUMER_H_

// Required if you do not compile with corresponding libraries (-> shorter cmake file)
// Define before importing CImg.h to disable viewer
#define cimg_display 0 

#include <mutex>
#include "CImg.h"
#include "procon.h"
#include "Point.h"
#include "config.h"



/*
 * Hier findet die tatsaechliche Berechnung der Daten statt
 */
class FernConsumer: public Consumer<Point> {
    public:
     FernConsumer(Buffer<Point>& buffer);
     ~FernConsumer();
     void save();
    private:
     static std::mutex mutexes[];
     static std::mutex save_mutex;
     static cimg_library::CImg<unsigned char> m_image;     
     static bool saved;

     bool consume(Point& datapoint);
     void convert(Point& datapoint);

};

#endif