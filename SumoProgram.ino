/**
 * The program is a sumo robot named toaster tank. This robot wants to grow up to be a toaster
 * 
 */
#include <Wire.h>
#include <Zumo32U4.h>

#include "UpdateData.h"
#include "Motors.h"
#include "RobotProcess.h"
#include "FightProcesses.h"


const double LEFT_MULTIPLIER = .943;
const double RIGHT_MULTIPLIER = 1.0;


Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;    // http://pololu.github.io/zumo-32u4-arduino-library/class_zumo32_u4_button_a.html
//Zumo32U4Buzzer buzzer;
Zumo32U4LineSensors lineSensors;    // the sensors under the robot
Zumo32U4ProximitySensors proxSensors; // The sensors that detect around the robot


Motors motors;
RobotProcess* process = NULL;

void setup() {
    Serial.begin(9600);
    delay(1000);  // if the serial monitor is up, wait for connection, if not connect after 1 second, don't worry
    Serial.println();
    Serial.println();
    Serial.println(F("starting new, waiting for a press"));
    
    lcd.clear();
    lcd.print(F("Press a!"));
    buttonA.waitForButton();    // don't start until user presses A
    lcd.clear();
    Serial.println(F("A pressed, going to loop function"));
    
    lineSensors.initThreeSensors();
    proxSensors.initThreeSensors();
//    process = new TurnProcess(90, new MoveProcess(30, new TurnProcess(-90, new MoveProcess(10, new TurnProcess(-180, NULL)))));;
    process = new EscapeProcess(2);
}

void loop() {
    UpdateData data (&lineSensors, &proxSensors, &motors, &lcd);
//    data.debugLineSensors(&lcd);
//    data.debugProxSensors(&lcd);

//    data.getMotors()->spinAmount(360);
//    delay(1000);
//    data.getMotors()->stopMotors();
//    delay(1000);
//    return;
    if (process != NULL){
        process->update(&data);
        if (process->isDone()){
            
            RobotProcess* nextProcess = process->getNextProcess();
//            if (nextProcess){
//                boolean shouldDelete = false;
//                RobotProcess* nextFinal = nextProcess;
//                while ((nextFinal = nextFinal->getNextProcess())){
//                    if (nextFinal == process){
//                        shouldDelete = true;
//                        break;
//                    }
//                }
//                if (shouldDelete){
//                    delete process;  // delete only if     
//                }
//            }
            delete process;
            process = nextProcess;
            
        }
    }
}







