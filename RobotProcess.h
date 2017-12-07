#ifndef RobotProcess_H
#define RobotProcess_H


#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>

#include "UpdateData.h"


class RobotProcess{
    private:
        boolean started = false;  // has the update method been called yet?
        long startTime = 0;  // the start time in ms or 0 if started is false
    protected:
        RobotProcess* nextProcess = NULL;  // the next process that will be used when this is done
        boolean ended = false;  // has this ended yet? Subclasses can change this when their process is done

    protected:  // functions that subclasses should override
        /** 
         * Called when right before the first update has been called.
         * If onStart has been called before, this will only be called if started is false meaning that\
         *      this will only be called if this process has changed to something else, then changed back to this
         */ 
        virtual void onStart(UpdateData* data)=0;
        /**
         * Called after onUpdate when ended is set to true
         */
        virtual void onEnd(UpdateData* data){}
        /**
         * Called in a loop whenever ended is true
         */
        virtual void onUpdate(UpdateData* data)=0;
    
    public:
        RobotProcess(){}
        virtual ~RobotProcess();

        RobotProcess* getNextProcess();
        void setNextProcess(RobotProcess* nextProcess);
        boolean isDone();
        long getStartTime();

        void update(UpdateData* data);
};

/**
 * A TimedProcess keeps track of how long something should take, and it's up for the subclasses to set ended to true normally\
 *      when a call to getPercentThere() is >= 1
 */
class TimedProcess : public RobotProcess {
    protected:
        long duration;
    public:
        TimedProcess(){}
        ~TimedProcess(){}
        /**
         * Returns a value between 0 and 1, potentially > 1
         */
        double getPercentThere();
};

class MoveProcess : public TimedProcess{
    private:
        int distance; // the distance in cm
        double turnMultiplier;

    public:
        MoveProcess(int distance, double turnMultiplier, RobotProcess* nextProcess);
        ~MoveProcess(){}

    protected:
        void onStart(UpdateData* data) override;
        void onUpdate(UpdateData* data) override;
};

class TurnProcess : public TimedProcess{

    private:
        float turnAmount;
    
    public:
        TurnProcess(float turnAmount, RobotProcess* nextProcess);
        ~TurnProcess(){}

    protected:
        void onStart(UpdateData* data) override;
        void onUpdate(UpdateData* data) override;
        
};



#endif
