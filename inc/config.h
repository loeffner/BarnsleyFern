#ifndef _CONFIG_H_
#define _CONFIG_H_


#define NR_PRODUCER 6           // Nr of producer-threads
#define NR_CONSUMER 4           // Nr of consumer-threads

#define BUFFER_SIZE 1000        // Size of the FiFo Buffer
#define PRODUCER_BATCH_SIZE 100 // Points are generated in batches
#define CONSUMER_BATCH_SIZE 100 // Points are consumed in batches


#define IMG_PATH "Fern.png"     // Path of image

#define IMG_MUTEX_CNT 100       // Image is separated into a grid, each mutex safes a row
#define NR_POINTS 100000000     // Nr of points per producer
#define IMG_WIDTH 10000         // Image width
#define IMG_HEIGHT 20000        // Image height

// Data window
#define X_MIN -2.2               
#define X_MAX 2.7
#define Y_MIN 0.0
#define Y_MAX 10.0

#endif