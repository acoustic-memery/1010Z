/** @file auto.c
 * @brief File for autonomous code
 *
 * This file should contain the user autonomous() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

/*
 * Runs the user autonomous code. This function will be started in its own task with the default
 * priority and stack size whenever the robot is enabled via the Field Management System or the
 * VEX Competition Switch in the autonomous mode. If the robot is disabled or communications is
 * lost, the autonomous task will be stopped by the kernel. Re-enabling the robot will restart
 * the task, not re-start it from where it left off.
 *
 * Code running in the autonomous task cannot access information from the VEX Joystick. However,
 * the autonomous function can be invoked from another task if a VEX Competition Switch is not
 * available, and it can access joystick information if called in this way.
 *
 * The autonomous task may exit, unlike operatorControl() which should never exit. If it does
 * so, the robot will await a switch to another mode or disable/enable cycle.
 */
#define FOREWARD 1
#define BACKWARDS 2
#define RIGHT 1
#define LEFT 2
#define UP 1
#define DOWN 2
#define OPEN 1
#define CLOSED 2

int clawClosed = 500;

int clawOpen = 4000;

int chainForeward = 4000;

int chainBackward = 200;

void motorsForeward (float output) {

}

void motorsBackward (float output) {

}

void motorsRight () {

}

void motorsLeft () {

}

void motorsStop () {

}

void motorsLiftUp (int power) {

}

void motorsLiftDown (int power) {

}

void motorsLiftStop () {


}

void motorsChainBack () {


}

void motorsChainForeward () {


}

void motorsChainStop () {


}

void motorsClawOpen () {


}

void motorsClawClosed () {


}

void motorsClawStop () {


}

void stopEverything() {
    motorsStop();
    clawStop();
    chainStop();
  }

  #define INTEGRAL 1
  #define DERIVATIVE 0.0


  void go(int way, int distance) {
    encoderReset(driveEncLeft);
    encoderReset(driveEncRight);

    float encTickLastLeft;
    float encTickLastRight; //tick last left and right


    float outputLeft;
    float outputRight; //power to be outputed left and right

    float errLeft;
    float errRight; //target distance - actual distance left and right

    float errTotalRight;
    float errTotalLeft; //total accumulated error left and right

    int errAccuLeft = 50;
    int errAccuRight = 50; //total accumulated error left and right if less than integral cap

    float gain = 0.25; //motor gain for proportional term

    float integCap = 50/INTEGRAL; //integral cap

    float propLeft;
    float propRight; //calculated proportional term left and right
    float integLeft;
    float integRight; //calculated integral term left and right
    float derivLeft;
    float derivRight; //calculated derivative term left and right

    int motorAdjust = 5; //motor adjustment amount



    while(((abs(encoderGet(driveEncLeft)) +
(abs(encoderGet(driveEncRight)))) / 2) < distance) { //while less than target distance



       errLeft = distance - (abs(encoderGet(driveEncLeft)));
       errRight = distance - (abs(encoderGet(driveEncRight)));
//calculate error

       if(errRight < errAccuRight) {errTotalRight += errRight;}
       else {errTotalRight = 0;}

       if(errLeft < errAccuLeft) {
         errTotalLeft += errLeft;}
       else {errTotalLeft = 0;} //makes integral term 0 if error is less than 50 (this has not been properly tuned)

       if(errTotalLeft > integCap) {
         errTotalLeft = 50/INTEGRAL;
       }

       if(errTotalRight > integCap) {
         errTotalRight = 50/INTEGRAL; //if integral term is greater than 50 (after previous calculation) integral term is incorperated
       }

//the above part of the function is only for managing when the integral is enganged

       propLeft = errLeft * gain;
       propRight = errRight * gain; //calculate proprtional term

       integLeft = errTotalLeft * INTEGRAL;
       integRight = errTotalRight * INTEGRAL; //calculate integral term

       derivLeft = (errLeft - encTickLastLeft) * DERIVATIVE;
       derivRight = (errRight - encTickLastRight) * DERIVATIVE;
//calculate derivative term

encTickLastRight = errRight;
encTickLastLeft = errLeft; //calculate enctick last

outputLeft = propLeft + integLeft + derivLeft;
outputRight = propRight + integRight + derivRight; //output P + I + D
for both sides

if (outputLeft > 127) {
   outputLeft = 127;
}

if (outputRight > 127) {
   outputRight = 127;
}

//make output 127 if greater than 127

if(way == 1) {
//if forewards

//adjusts motors to make both sides turn at approx the same speed

   if (outputLeft < (outputRight +- 20)) {
     motorSet(1, (outputLeft + motorAdjust));
     motorSet(4, -(outputLeft + motorAdjust)); // left side

     motorSet(7, outputRight);
     motorSet(10, -(outputRight)); //right side
   }
   if (outputRight < (outputLeft +- 20)) {
     motorSet(1, (outputLeft));
     motorSet(4, -(outputLeft)); // left side

     motorSet(7, (outputRight + motorAdjust));
     motorSet(10, -(outputRight + motorAdjust));

   }
   else {
     motorSet(1, outputLeft);
     motorSet(4, -(outputLeft));
     motorSet(7, (outputRight));
     motorSet(10, -(outputRight));
   }

}

if(way == 2) {
//backwards

//adjusts motors to keep both sides the same speed
   if (outputLeft < (outputRight +- 20)) {
     motorSet(1, -(outputLeft + motorAdjust));
     motorSet(4, (outputLeft + motorAdjust)); // left side

     motorSet(7, -(outputRight));
     motorSet(10, (outputRight)); //right side
   }
   if (outputRight < (outputLeft +- 20)) {
     motorSet(1, -(outputLeft));
     motorSet(4, (outputLeft)); // left side

     motorSet(7, -(outputRight + motorAdjust));
     motorSet(10, (outputRight + motorAdjust));

   }
   else {
     motorSet(1, -(outputLeft));
     motorSet(4, (outputLeft));
     motorSet(7, -(outputRight));
     motorSet(10, (outputRight));
   }


}


delay(20);

    }



  }

void liftUp (int height, int speed) {
  encoderReset(armEnc);
  encoderGet(armEnc);

  while (abs(encoderGet(armEnc)) < height) {
    motorsLiftUp(speed);
    encoderGet(armEnc);
    delay(20);
  }
  if (abs(encoderGet(armEnc)) >= height) {
    motorsLiftStop();
  }
  encoderReset(armEnc)
}

void liftDown (int amount, int speed) {
  encoderReset(armEnc);
  encoderGet(armEnc);

  while (abs(encoderGet(armEnc)) < height) {
    motorsLiftUp(speed);
    encoderGet(armEnc);
    delay(20);
  }
  if (abs(encoderGet(armEnc)) >= height) {
    motorsLiftStop();
  }
  encoderReset(armEnc)
}



void claw (int position) {

  if (position = 1) {
    analogRead(1);

    while (analogRead(1) < clawOpen) {
      analogRead(1);
      motorsClawOpen();
      delay(20);
    }
    if (analogRead(1) >= clawOpen) {
      motorsClawStop();
    }
  } // if open

  else if (position = 2) {
    analogRead(1);
    while (analogRead(1) > clawClosed) {
      analogRead(1);
      motorsClawClosed();
      delay(20);
    }
    if (analogRead(1) <= clawClosed) {
      motorsClawStop();
    }
  } //if closed

}

void chain (int position) {

  if (position = 1) {
    analogRead(2);

    while (analogRead(2) < chainForeward) {
      analogRead(2);
      motorsChainBack()
      delay(20);
    }
    if (analogRead(2) >= chainForeward) {
      motorsChainStop();
    }
  } // if back

  else if (position = 2) {
    analogRead(2);
    while (analogRead(2) > chainBackward) {
      analogRead(2);
      motorsChainForeward();
      delay(20);
    }
    if (analogRead(2) <= chainBackward) {
      motorsChainStop();
    }
  } //if forewards

}

void skillsAuto () {

}

void compLeftAuto () {


}

void compRightAuto () {

  
}

void autonomous() {
  if (program = 1) {
    skillsAuto();
  }
  else if (program = 2) {
    compLeftAuto();
  }
  if (program = 3) {
    compRightAuto();
  }

}
