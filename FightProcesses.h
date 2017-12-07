#ifndef FightProcesses_H
#define FightProcesses_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>

#include "RobotProcess.h"

RobotProcess* isOverLine(UpdateData* data, int currentLevels);


/**
 * You don't always want to attack so instead, you should escape, then attack when you are ready. 
 *      If you are using this process, you aren't ready to attack
 */
class EscapeProcess : public RobotProcess{

    private:
        static const int ESCAPE_SPEED = 280;
        static const int DANGER_ESCAPE_SPEED = 350;
        int levels;
    
    public:
        /**
         * 
         * @param levels: Normally 0, when it isn't zero, the next process will be another EscapeProcess and it will be passed levels - 1\
         *      this is useful if you want to go into escape mode, bounce off the wall, escape mode again, then something else
         */
        EscapeProcess(int levels = 0);
        ~EscapeProcess(){}

    protected:
        void onStart(UpdateData* data);
        void onUpdate(UpdateData* data);
};


/**
 * The process that should be used to get to the middle. At the end, you most likely will want to go into search mode
 */
class MoveMiddle : public MoveProcess{
    
    public:
        MoveMiddle();
        ~MoveMiddle(){}

    protected:
        void onStart(UpdateData* data) override;
        void onUpdate(UpdateData* data) override;
        void onEnd(UpdateData* data) override;
};


/**
 * Process that should be used when you are in a safe area so you can search and find your opponent
 */
class SearchProcess : public TimedProcess {
    protected:
        boolean careful;

    public:
        /**
         * @param careful. This affects how long the robot will search, affects its reaction if there is a robot near it on either side
         */
        SearchProcess(boolean careful = false);
        ~SearchProcess(){}
    private:
        /**
         * Makes the robot start, or keep turning in a specific direction
         * @param turnDirection: Should be -1, 0, or 1. 1 indicates a spin to the left, 
         */
        void turnDirection(int turnDirection);
    protected:
        void onStart(UpdateData* data) override;
        void onUpdate(UpdateData* data) override;
        void onEnd(UpdateData* data) override;
};

class RamProcess : public TimedProcess {
    private:
        static const int RAM_SPEED = 400;
    public:
        RamProcess();
        ~RamProcess(){}

    protected:
        void onStart(UpdateData* data) override;
        void onUpdate(UpdateData* data) override;
        void onEnd(UpdateData* data) override;
};




#endif
