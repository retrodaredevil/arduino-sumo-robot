#include "FightProcesses.h"

long lastBounce = 0;

RobotProcess* isOverLine(UpdateData* data, int currentLevels){
    
    boolean left = data->isLeftOver();
    boolean center = data->isCenterOver();
    boolean right = data->isRightOver();
    if (left || center || right){
        float turnAmount = 180;
        double backingTurnMult = 0;
        if (left && (!center && !right)){
            backingTurnMult = .5;  // slow down left motor while backing by 70%
            turnAmount = -130;  // it would be faster to turn clockwise
        } else if (right && (!center && !left)){
            backingTurnMult = -.5; // slow down right motor while backing by 70%
            turnAmount = 130;
        }
        RobotProcess* laterProcess = NULL;
        if (currentLevels > 0){
//            turnAmount *= .8;  // if we are't going towards the center, make it go to the side more
            laterProcess = new EscapeProcess(currentLevels - 1);
        } else {
            laterProcess = new MoveMiddle();
        }
        long now = millis();
        if (now - lastBounce < 2000){
            turnAmount *= .5;  // if we didn't drive for very long, then we probably are turning too much
        }
        
        lastBounce = now;
        return new MoveProcess(-18/*cm*/, backingTurnMult, new TurnProcess(turnAmount, laterProcess));
    }
    return NULL;
}
/**
 * @param farthestDistance: a number 0 to 6 where 6 is closer
 */
boolean willHaveCollision(byte frontLeft, byte frontRight, int farthestDistance = 5){ 
    return frontLeft >= farthestDistance && frontRight >= farthestDistance && abs(frontRight - frontLeft) <= 1; 
}


EscapeProcess::EscapeProcess(int levels): levels(levels){}

void EscapeProcess::onStart(UpdateData* data){}

void EscapeProcess::onUpdate(UpdateData* data){
//    data->debug("es: " + String(levels));
    
    byte frontRight = data->getFrontRight();
    byte frontLeft = data->getFrontLeft();
    byte rightSide = data->getRightSide();
    byte leftSide = data->getLeftSide();
    
    // what should we do it we are going to have a collision if we keep going
    if (willHaveCollision(frontLeft, frontRight)){  
        setNextProcess(new TurnProcess(90, new EscapeProcess(0)));
        ended = true;
        return;    
    }
    int currentSpeed = ESCAPE_SPEED;
    if (rightSide >= 5 || leftSide >= 5){
        currentSpeed = DANGER_ESCAPE_SPEED;
    }
    data->getMotors()->setSpeeds(currentSpeed, currentSpeed);
    
    
    RobotProcess* nextProcess = isOverLine(data, levels);
    if (nextProcess){
        setNextProcess(nextProcess);  //escape mode now
        ended = true;
    }
}

//MoveMiddle
MoveMiddle::MoveMiddle(): MoveProcess(28, 0, NULL) {}

void MoveMiddle::onStart(UpdateData* data){
    MoveProcess::onStart(data);
}


void MoveMiddle::onUpdate(UpdateData* data){
    MoveProcess::onUpdate(data);
    double wayThere = getPercentThere();
//    data->debug(String(wayThere));
    RobotProcess* nextProcess = isOverLine(data, 1);
    if (nextProcess){
        setNextProcess(nextProcess);  // we should be in escape mode now
        ended = true;
    }
}
void MoveMiddle::onEnd(UpdateData* data){ 
    // since we are now in the middle, we probably don't have a nextProcess so, we want to start searching
    if (!nextProcess){
        // go into search mode
        setNextProcess(new SearchProcess(false));
    }
}



SearchProcess::SearchProcess(boolean careful) : careful(careful) {
    duration = careful ? 1500 : 3000;
}
void SearchProcess::onStart(UpdateData* data){ 

        data->getMotors()->spinAmount(1);  // we should start spining to the left
}
void SearchProcess::onUpdate(UpdateData* data){
    if (getPercentThere() >= 1){
        ended = true;
        return;
    }
    
    RobotProcess* nextProcess = isOverLine(data, 1);  // just check just in case
    if (nextProcess){
        setNextProcess(nextProcess);
        ended = true;
        return;
    }
    // Now we don't have to worry about lines, and can start spinning to our surroundings
    byte frontRight = data->getFrontRight();
    byte frontLeft = data->getFrontLeft();
    byte rightSide = data->getRightSide();
    byte leftSide = data->getLeftSide();
    
    if (rightSide == 6 || leftSide == 6){  //we are going to get destroyed if we don't get out of here.
        setNextProcess(new EscapeProcess(1));
        ended = true;
        return;
    }
    if (leftSide >= 2){
        data->getMotors()->spinAmount(1);  // we should start spining to the left
    } else if (rightSide >= 2){
        data->getMotors()->spinAmount(-1);  //start spinning to the right
    }

    // 4 is about 14 inches
    const int FARTHEST_DISTANCE = 4;  // lower number if farther away. This can be 0 to 6
    if(willHaveCollision(frontLeft, frontRight, FARTHEST_DISTANCE)){
        // ram it
//        setNextProcess(new TurnProcess(360, new EscapeProcess(0)));  // temporary until we create RamProcess
        setNextProcess(new RamProcess());
        ended = true;
    } else if (frontRight >= FARTHEST_DISTANCE && frontLeft >= FARTHEST_DISTANCE){  // is there still something in front of us?
//        data->debug(F("Adjustin"), String(millis() % 1000));
        if (frontLeft > frontRight){ // there is something closer to the left sensor
            data->getMotors()->spinAmount(1);
        } else{
            data->getMotors()->spinAmount(-1);
        }
    }
    
}

void SearchProcess::onEnd(UpdateData* data){
    if(!nextProcess){
        setNextProcess(new EscapeProcess(0));
    }
}



RamProcess::RamProcess(){
    duration = 10000;  // ram for 10 seconds
}

void RamProcess::onStart(UpdateData* data){}
void RamProcess::onUpdate(UpdateData* data){
    if (getPercentThere() >= 1){
        ended = true;
        return;
    }
    RobotProcess* nextProcess = isOverLine(data, 1);
    if (nextProcess){
        setNextProcess(nextProcess);
        ended = true;
        return;
    }

    byte frontRight = data->getFrontRight();
    byte frontLeft = data->getFrontLeft();
    byte rightSide = data->getRightSide();
    byte leftSide = data->getLeftSide();
    
    double turnMult = (frontLeft - frontRight) / 2.;  // something closer to left, turn left, and vice versa
    data->getMotors()->goDistance(1, turnMult, false, RAM_SPEED);
    if (frontRight <= 2 && frontLeft <= 2){
        setNextProcess(new EscapeProcess(1));
        ended = true;
        return;
    }
}

void RamProcess::onEnd(UpdateData* data){
    if (!nextProcess){
        setNextProcess(new MoveProcess(-27, .8, new SearchProcess())); // back and turn, then search, maybe we'll be s
    }
}






