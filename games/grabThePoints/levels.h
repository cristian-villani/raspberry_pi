#ifndef LEVELS_H
#define LEVELS_H

// constants
// extern const int NUM_LEVELS;
// extern const int LEVEL_HEIGHT;
// extern const int LEVEL_WIDTH;
const int NUM_LEVELS  = 3;
const int LEVEL_HEIGHT = 20;
const int LEVEL_WIDTH  = 63;
const int NUM_OBJ  = 10;

extern int nPoints;
extern int nColl;
extern int nLife;
extern double seconds;


// levels array
extern char levels[NUM_LEVELS][LEVEL_HEIGHT][LEVEL_WIDTH + 1];  // use the constants
#endif

