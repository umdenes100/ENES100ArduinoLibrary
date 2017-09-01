#include "Enes100.h"

Coordinate::Coordinate() {
    init(0, 0, 0);
}

Coordinate::Coordinate(double x, double y) {
    init(x, y, 0);
}

Coordinate::Coordinate(double x, double y, double theta) {
    init(x, y, theta);
}

void Coordinate::init(double x, double y, double theta) {
    this->x = x;
    this->y = y;
    this->theta = theta;
}

Enes100::Enes100(const char* teamName, int teamType, int markerId, int rxPin, int txPin) {
    mId = markerId;
    mSoftwareSerial = new SoftwareSerial(rxPin, txPin);
    mSoftwareSerial->begin(9600);
    
    mSoftwareSerial->print("#start ");
    mSoftwareSerial->print(teamType);
    mSoftwareSerial->print(" ");
    mSoftwareSerial->print(teamName);
    mSoftwareSerial->print("*");
    mSoftwareSerial->flush();
}

// MARK: baseObjective
void Enes100::baseObjective(int value) {
    mSoftwareSerial->print("#base ");
    mSoftwareSerial->print(value);
    mSoftwareSerial->print("*");
    mSoftwareSerial->flush();
}

void Enes100::baseObjective(double value) {
    mSoftwareSerial->print("#base ");
    mSoftwareSerial->print(value);
    mSoftwareSerial->print("*");
    mSoftwareSerial->flush();
}

void Enes100::baseObjective(Coordinate& value) {
    mSoftwareSerial->print("#base ");
    mSoftwareSerial->print(value.x);
    mSoftwareSerial->print(",");
    mSoftwareSerial->print(value.y);
    mSoftwareSerial->print("*");
    mSoftwareSerial->flush();
}

// MARK: bonusObjective
void Enes100::bonusObjective(int value) {
    mSoftwareSerial->print("#bonus ");
    mSoftwareSerial->print(value);
    mSoftwareSerial->print("*");
    mSoftwareSerial->flush();
}

void Enes100::bonusObjective(double value) {
    mSoftwareSerial->print("#bonus ");
    mSoftwareSerial->print(value);
    mSoftwareSerial->print("*");
    mSoftwareSerial->flush();
}

// MARK: endMission
void Enes100::endMission() {
    mSoftwareSerial->print("#end*");
    mSoftwareSerial->flush();
    while(1);
}

// MARK: navigated
void Enes100::navigated() {
    mSoftwareSerial->print("#navigated*");
    mSoftwareSerial->flush();
}

// MARK: print
void Enes100::print(const char *msg) {
    mSoftwareSerial->print(msg);
    mSoftwareSerial->flush();
}

void Enes100::print(int msg) {
    mSoftwareSerial->print(msg);
    mSoftwareSerial->flush();
}

void Enes100::print(double msg) {
    mSoftwareSerial->print(msg);
    mSoftwareSerial->flush();
}

// MARK: println
void Enes100::println(const char *msg) {
    mSoftwareSerial->println(msg);
    mSoftwareSerial->flush();
}

void Enes100::println(int msg) {
    mSoftwareSerial->println(msg);
    mSoftwareSerial->flush();
}

void Enes100::println(double msg) {
    mSoftwareSerial->println(msg);
    mSoftwareSerial->flush();
}

// MARK: retrieveDestination
bool Enes100::retrieveDestination() {
    mSoftwareSerial->print("#destination*");
    mSoftwareSerial->flush();
    
    unsigned long start = millis();
    int state = 0;
    
    while((millis() - start) < 600) {
        if (mSoftwareSerial->available()) {
            switch(state) {
                case 0:
                    destination.x = mSoftwareSerial->parseFloat();
                    state++;
                    break;
                case 1:
                    destination.y = mSoftwareSerial->parseFloat();
                    state++;
                    break;
                case 2:
                    return true;
                    break;
                default:
                    return false;
                    break;
            }
        }
    }
    
    return false;
}

// MARK: updateLocation
unsigned long Enes100::updateLocation() {
    mSoftwareSerial->print("#");
    mSoftwareSerial->print(mId);
    mSoftwareSerial->print("*");
    mSoftwareSerial->flush();
    
    unsigned long start = millis();
    int state = 0;
    
    while((millis() - start) < 600) {
        if(mSoftwareSerial->available()) {
            switch(state) {
                case 0:
                    if (mSoftwareSerial->parseInt() == mId)
                        state++;
                    break;
                case 1:
                    location.x = mSoftwareSerial->parseFloat();
                    state++;
                    break;
                case 2:
                    location.y = mSoftwareSerial->parseFloat();
                    state++;
                    break;
                case 3:
                    location.theta = mSoftwareSerial->parseFloat();
                    return millis() - start;
                    break;
                default:
                    return 0;
                    break;
            }
        }
    }
    
    return 0;
}
