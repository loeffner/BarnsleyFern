#ifndef _PRODUCER_H_
#define _PRODUCER_H_

#include "Worker.h"
#include "Buffer.h"

#define PACKET_SIZE 100
/*
 * Producer kuemmert sich um das Verarbeiten der erzeugten Daten.
 * Die eigentliche Berechnung der Daten findet in FernProducer statt.
 */
template <class T> class Producer : public Worker // Template Klasse zur Unterstuetzung unterschiedlicher Datenpunkte
{
    public:
     Producer(Buffer<T>& buffer);               // Constructor
     ~Producer();                               // Destructor
    protected:
     virtual bool produce(T& datapoint) = 0;    // Hier werden in der Kindklasse die eigentlichen Daten produziert
     virtual bool step();                       // Hier wird der Datenpunkt an den Buffer uebergeben
    private:
     Buffer<T>& m_buffer;                        // Ein geeigneter Buffer
};

#endif