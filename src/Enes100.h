#ifndef Enes100_h
#define Enes100_h

#include "VisionSystemClient.hpp"

#define CRASH_SITE        0
#define DATA              1
#define MATERIAL          2
#define FIRE              3
#define WATER             4

#define DIRECTION         0
#define LENGTH            1
#define HEIGHT            2
#define POS_X             0
#define NEG_X             1
#define POS_Y             2
#define NEG_Y             3

#define CYCLE             0
#define MAGNETISM         1
#define MAGNETIC          0
#define NOT_MAGNETIC      1

#define WEIGHT            0
#define MATERIAL_TYPE     1
#define FOAM              0
#define PLASTIC           1
#define HEAVY             0
#define MEDIUM            1
#define LIGHT             2

#define NUM_CANDLES       0
#define TOPOGRAPHY        1
#define TOP_A             0
#define TOP_B             1
#define TOP_C             2

#define DEPTH             0
#define WATER_TYPE        1
#define FRESH_UNPOLLUTED  0
#define FRESH_POLLUTED    1
#define SALT_UNPOLLUTED   2
#define SALT_POLLUTED     3


VisionSystemClient Enes100;

#endif /* Enes100_h */
