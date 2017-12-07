#ifndef UpdateData_H
#define UpdateData_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>

#include "Motors.h"

/*
 * A class that holds data neccessary for other files to use. A new one should be created every 'frame'
 */
class UpdateData {
    private:
        // [0] is right, [1] is middle, [2] is left. A value lower than 300 is white
        // value at [1] (center) seems to always be lower than the other two
        unsigned int * sensorValues;  // size of 3, values between 0 and 1000, lower being white

        // a value of 4 is 14 inches, and a value higher, is closer but it's hard to tell how close
        byte frontLeft;  // a distance
        byte frontRight;  // a distance

        // value of 6 is 3 inches, 5 is 3-7 inches, 4 is 8-10, 3 is 12-16 inches, 2 is >16
        byte right;  // rightValue
        byte left;  // leftValue

        Motors* motors;
        Zumo32U4LCD* lcd;

    public:
//        UpdateData(unsigned int lineSensorValues[], byte frontLeftValue, byte frontRightValue, 
//                   byte rightValue, byte leftValue);
        UpdateData(Zumo32U4LineSensors* lineSensors, Zumo32U4ProximitySensors* proxSensors, Motors* motors, Zumo32U4LCD* lcd);
                   
        boolean isLeftOver();
        boolean isCenterOver();
        boolean isRightOver();

        byte getFrontLeft();
        byte getFrontRight();
        byte getRightSide();
        byte getLeftSide();

        Motors* getMotors();

        void debug(String first, String second = "");
        void debugLineSensors();
        void debugProxSensors();
//        static UpdateData UpdateData::createWithSensors(Zumo32U4LineSensors* lineSensors, 
//                                                        Zumo32U4ProximitySensors* proxSensors);
};

#endif
