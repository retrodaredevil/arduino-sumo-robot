#include "UpdateData.h"

//UpdateData::UpdateData(unsigned int lineSensorValues[], byte frontLeftValue, byte frontRightValue, 
//                       byte rightValue, byte leftValue){
//    sensorValues = lineSensorValues;
//    frontLeft = frontLeftValue;
//    frontRight = frontRightValue;
//    right = rightValue;
//    left = leftValue;
//}
UpdateData::UpdateData(Zumo32U4LineSensors* lineSensors, Zumo32U4ProximitySensors* proxSensors, Motors* motors, Zumo32U4LCD* lcd):
        motors(motors), lcd(lcd) {
    unsigned int lineSensorValues[3];    // create an array with a length of 3
    lineSensors->read(lineSensorValues);    // read and put values in array
    sensorValues = lineSensorValues;

    proxSensors->read(); // send out pulses (The calls to proxSensors->counts*() will use this)
    frontLeft = proxSensors->countsFrontWithLeftLeds(); 
    frontRight = proxSensors->countsFrontWithRightLeds();
    left = proxSensors->countsLeftWithLeftLeds();
    right = proxSensors->countsRightWithRightLeds();
    
}

/**
 * @return true if the front left detects white, false otherwise
 */
boolean UpdateData::isLeftOver(){ return sensorValues[0] < 300; } // is left sensor white
/**
 * @return true if the front center detects white, false otherwise
 */
boolean UpdateData::isCenterOver(){ return sensorValues[1] < 200; }
/**
 * @return true if the front right detects white, false otherwise
 */
boolean UpdateData::isRightOver(){ return sensorValues[2] < 300; } 

byte UpdateData::getFrontLeft(){ return frontLeft; }
byte UpdateData::getFrontRight(){ return frontRight; }
byte UpdateData::getLeftSide(){ return left; }
byte UpdateData::getRightSide(){ return right; }

Motors* UpdateData::getMotors(){ return motors; }

void UpdateData::debug(String first, String second){
    lcd->clear();
    lcd->gotoXY(0, 0);
    lcd->print(first);
    lcd->gotoXY(0, 1);
    lcd->print(String(second));    
}

void UpdateData::debugLineSensors(){    
    debug(String(sensorValues[0]) + "," + String(sensorValues[1]), String(sensorValues[2]));    
}
void UpdateData::debugProxSensors(){
    debug("F" + String(frontLeft) + "," + String(frontRight) + "L" + String(left) + "R" + String(right));
}
 

  
