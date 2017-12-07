#include "RobotProcess.h"


RobotProcess::~RobotProcess(){}

RobotProcess* RobotProcess::getNextProcess(){ return nextProcess; }
void RobotProcess::setNextProcess(RobotProcess* nextProcess) { this->nextProcess = nextProcess; }

boolean RobotProcess::isDone(){ return ended; }
long RobotProcess::getStartTime(){ return startTime; }

void RobotProcess::update(UpdateData* data){
    if (!started){
        started = true;
        startTime = millis();
        onStart(data);
    }
    onUpdate(data);
    if (ended){
        onEnd(data);
        started = false;
    }
}

double TimedProcess::getPercentThere(){
    // we add 7 since, usually, it's better to end early, than to go too far.
    return ((double)(millis() + 7)) / (duration + getStartTime());
}



MoveProcess::MoveProcess(int distance, double turnMultiplier, RobotProcess* nextProcess) : distance(distance), turnMultiplier(turnMultiplier){
    setNextProcess(nextProcess);
}
void MoveProcess::onStart(UpdateData* data){
    duration = data->getMotors()->goDistance(distance, turnMultiplier);
}
void MoveProcess::onUpdate(UpdateData* data){
    if (getPercentThere() >= 1){
        ended = true;
        data->getMotors()->stopMotors();
    }
}


TurnProcess::TurnProcess(float turnAmount, RobotProcess* nextProcess): turnAmount(turnAmount){
    setNextProcess(nextProcess);    
}

void TurnProcess::onStart(UpdateData* data){
    duration = data->getMotors()->spinAmount(turnAmount);
}
void TurnProcess::onUpdate(UpdateData* data){
    if (getPercentThere() >= 1){  // add 7 in case it getting here takes some time
        ended = true;
        data->getMotors()->stopMotors();
    }
}

