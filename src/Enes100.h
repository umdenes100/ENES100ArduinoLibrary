#ifndef Enes100_h
#define Enes100_h

#include "VisionSystemClient.hpp"

#define CRASH_SITE        0
#define DATA              1
#define MATERIAL          2
#define FIRE              3
#define WATER             4
#define SEED              5
#define HYDROGEN          6

// Crash Mission
#define DIRECTION         0
#define LENGTH            1
#define HEIGHT            2
#define NORMAL_X          0
#define NORMAL_Y          1



// Data
#define CYCLE             0
#define MAGNETISM         1
#define MAGNETIC          0
#define NOT_MAGNETIC      1


// Materials
#define WEIGHT            0
#define MATERIAL_TYPE     1
#define FOAM              0
#define PLASTIC           1
#define HEAVY             0
#define MEDIUM            1
#define LIGHT             2


// Fire
#define NUM_CANDLES       0
#define TOPOGRAPHY        1
#define TOP_A             0
#define TOP_B             1
#define TOP_C             2


// Water
#define DEPTH             0
#define WATER_TYPE        1
#define FRESH_UNPOLLUTED  0
#define FRESH_POLLUTED    1
#define SALT_UNPOLLUTED   2
#define SALT_POLLUTED     3


// Seed
#define LOCATION 0
#define PERCENTAGE 1


//Hydrogen
#define VOLTAGE_OUTPUT 0
#define LED_COLOR 1
#define VOLTAGE_1 0
#define VOLTAGE_2 1
#define VOLTAGE_3 2
#define VOLTAGE_4 3
#define VOLTAGE_5 4
#define WHITE 0
#define RED 1
#define YELLOW 2
#define GREEN 3
#define BLUE 4



VisionSystemClient Enes100;

#endif /* Enes100_h */
