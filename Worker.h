#ifndef _WORKER_H_
#define _WORKER_H_

#include <thread>

/*
 * Basisklasse Worker: kuemmert sich um das Starten, 
 * Stoppen und regelmaessige Aufrufen einer Funktion
 */
class Worker {
    public:
     Worker();                      // Constructor
     virtual ~Worker();             // Constructor
     void start();                  // Starten des Threads
     void stop(bool);               // Vorzeitiges terminieren des Thread
    protected:                      
     virtual bool step() = 0;       // Rueckgabewert gibt an, ob der Thread beendet werden soll
    private:
     std::thread *m_thread;         // Speichert den Thread
     bool m_terminate;              // Soll Thread beendet werden?
     bool m_running;                // Laeuft Thread?
     void work();                   // Schleife die wiederholt step() aufruft
};

#endif