#ifndef Enes100_h
#define Enes100_h

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "Enums.h"

#ifndef Coordinate_h
#define Coordinate_h

#define abs(x) ((x)>0?(x):-(x))

class Coordinate {
public:
    double x;
    double y;
    double theta;
    Coordinate() {
        init(0, 0, 0);
    }

    Coordinate(double x, double y) {
        init(x, y, 0);
    }

    Coordinate(double x, double y, double theta) {
        init(x, y, theta);
    }
private:
    void init(double x, double y, double theta) {
        this->x = x;
        this->y = y;
        this->theta = theta;
    }
};
#endif

class Enes100 {
public:
    Enes100(const char* teamName, int teamType, int markerId, int rxPin, int txPin);
    void baseObjective(int value);
    void baseObjective(double value);
    void baseObjective(Coordinate& value);
    void bonusObjective(int value);
    void bonusObjective(double value);
    void endMission();
    void navigated();
    void print(const char* message);
    void print(int message);
    void print(double message);
    void println(const char* message);
    void println(int message);
    void println(double message);
    bool retrieveDestination();
    unsigned long updateLocation();
    
    Coordinate location;
    Coordinate destination;
    
private:
    int mId;
    SoftwareSerial* mSoftwareSerial;
};

#endif /* Enes100_h */
