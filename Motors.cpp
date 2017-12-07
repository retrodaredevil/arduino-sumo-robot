#include "Motors.h"

Motors::Motors(Zumo32U4Motors* motors) : motors(motors){}

Motors::Motors(): Motors(new Zumo32U4Motors()){}

void Motors::setSpeeds(int left, int right, boolean precise){
    double mult = 1;
    if (precise){
        int voltage = readBatteryMillivolts();
        mult = 4450. / voltage;
    }
//    int leftValue = 
//    int rightvalue = 

    motors->setSpeeds(left * LEFT_MULT * mult, right * RIGHT_MULT * mult);
}

void Motors::stopMotors(){
    setSpeeds(0, 0);
}
long Motors::goDistance(int cm, double turnMultiplier, boolean precise, int moveSpeed){
    int multiplier = cm < 0 ? -1 : 1;
    int speed = moveSpeed * multiplier;
    double leftMod = 1 - (turnMultiplier > 0 ? turnMultiplier : 0);
    double rightMod = 1 - (turnMultiplier < 0 ? -turnMultiplier : 0);

    
    setSpeeds(speed * leftMod, speed * rightMod, precise);
    return (abs(cm) * 1000) / 42 + 5;  // 42 cm per second
}

long Motors::spinAmount(float amountDegrees){
    int multiplier = amountDegrees < 0 ? -1 : 1;  // if 1, we want to turn counterclockwise
    setSpeeds(SPIN_SPEED * -multiplier, SPIN_SPEED * multiplier);
    return (abs(amountDegrees) * 1000) / 360 + 5;  // we can do this since we turn 360 degrees per second
}

