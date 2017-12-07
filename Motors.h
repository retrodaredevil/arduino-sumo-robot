
#ifndef Motors_H
#define Motors_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>


class Motors{
    private:
        static const int SPIN_SPEED = 324;
        static const int DISTANCE_MOVE_SPEED = 300;
        static const double LEFT_MULT = .943;
        static const double RIGHT_MULT = 1.0;

        Zumo32U4Motors* motors;

    public:
        Motors(Zumo32U4Motors* motors);
        Motors();

        /**
         * Sets the speed of the motors and uses LEFT and RIGHT MULT to ensure accuracy
         * 
         * @param precise:Normally true, if true, will lower speeds if needed to get same results with a battery with a lower charge
         */
        void setSpeeds(int left, int right, boolean precise = true);
        
        void stopMotors();

        /**
         * @param turnAmount: The amount to turn. If positive, slows down left motor to turn left, if negative, slows down right motors \
         *      to turn right. For instance, a value of 1 turns down the left motor by 100% stopping the left motor
         * @param precise: Normally true, if true, will lower speeds if needed to get same results with a battery with a lower charge
         * @param moveSpeed: The move speed of one of both the the motors. Note, changing the speed will not affect the value returned.
         * Returns the amount of time it will take to go 'cm' centimeters at a speed of 300
         */
        long goDistance(int cm, double turnMultiplier = 0, boolean precise = true, int moveSpeed = DISTANCE_MOVE_SPEED);

        /**
         * May not be completely accurate, but it tries to be
         * Note that this functions changes the speed of the motors once. If you call stopMotors() after\
         *      the returned milliseconds, the robot should be in the desired position.
         * This function does not enforce how long or how much it turns
         * 
         * @param amountDegrees: The amount in degrees you want the robot to turn. Positive is counterclockwise
         * @returns the amount of time neccessary until the robot will be in the desired position
         */
        long spinAmount(float amountDegrees);
};
#endif
