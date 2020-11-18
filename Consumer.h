#ifndef _CONSUMER_H_
#define _CONSUMER_H_

#include <stdexcept>
#include "Worker.h"
#include "Buffer.h"

/*
 * Producer kuemmert sich um das Verarbeiten der erzeugten Daten.
 * Die eigentliche Berechnung der Daten findet in FernProducer statt.
 */
template <class T> class Consumer : public Worker // Template Klasse zur Unterstuetzung unterschiedlicher Datenpunkte
{
    public:
     Consumer(Buffer<T>& buffer);               // Constructor
     ~Consumer();                               // Destructor
    protected:
     virtual bool consume(T& datapoint) = 0;        // Hier werden in der Kindklasse die eigentlichen Daten produziert
     virtual bool step();                       // Hier wird der Datenpunkt an den Buffer uebergeben
    private:
     Buffer<T>& m_buffer;                       // Ein geeigneter Buffer
};

#endif