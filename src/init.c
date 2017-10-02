/** @file init.c
 * @brief File for initialization code
 *
 * This file should contain the user initialize() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

/*
 * Runs pre-initialization code. This function will be started in kernel mode one time while the
 * VEX Cortex is starting up. As the scheduler is still paused, most API functions will fail.
 *
 * The purpose of this function is solely to set the default pin modes (pinMode()) and port
 * states (digitalWrite()) of limit switches, push buttons, and solenoids. It can also safely
 * configure a UART port (usartOpen()) but cannot set up an LCD (lcdInit()).
 */
void initializeIO() {
}
void lcdTask (void * parameter) {
int selection = 1;

   lcdInit(uart1);
   lcdClear(uart1);

   int autoId[] = {1, 2, 3, 4, 5};

     int counter = 0;
     lcdSetText(uart1, 2, "<      OK      >");
     while (selection == 1) {
         if (lcdReadButtons(uart1) == 4) {
             counter++;
             // lcdPrint(uart1, 1, "UP %d", counter);
         } else if (lcdReadButtons(uart1) == 1) {
             counter--;
             // lcdPrint(uart1, 1, "DOWN %d", counter);
         }

         if (autoId[counter] == 1) {
             lcdPrint(uart1, 1, "LOAD: Skills");
         } else if (autoId[counter] == 2) {
             lcdPrint(uart1, 1, "LOAD: Comp LS");
         } else if (autoId[counter] == 3) {
             lcdPrint(uart1, 1, "LOAD: Comp RS");
         }


         if (lcdReadButtons(uart1) == 2) {
             if (autoId[counter] == 1) {
                 program = 1;
         selection = 2;
         lcdClear(uart1);
         lcdPrint(uart1, 1, " confirmed");
                 break;
             } else if (autoId[counter] == 2) {
                 program = 2;
         selection = 2;
         lcdClear(uart1);
         lcdPrint(uart1, 1, " confirmed");
         break;
             } else if (autoId[counter] == 3) {
                 program = 3;
         selection = 2;
         lcdClear(uart1);
         lcdPrint(uart1, 1, " confirmed");
                 break;
             } else {
                 continue;
             }
              delay(120);
         }
         if (isAutonomous()){
         lcdClear(uart1);
         while (isAutonomous()){
         lcdPrint(uart1, 1, "Drive Enc Val");
         lcdPrint(uart1, 2, "Left Side %d", encoderGet(driveEncLeft));
         delay(20);
       }
       }
       }}



/*
 * Runs user initialization code. This function will be started in its own task with the default
 * priority and stack size once when the robot is starting up. It is possible that the VEXnet
 * communication link may not be fully established at this time, so reading from the VEX
 * Joystick may fail.
 *
 * This function should initialize most sensors (gyro, encoders, ultrasonics), LCDs, global
 * variables, and IMEs.
 *
 * This function must exit relatively promptly, or the operatorControl() and autonomous() tasks
 * will not start. An autonomous mode selection menu like the pre_auton() in other environments
 * can be implemented in this task if desired.
 */
void initialize () {
  TaskHandle lcdTaskHandle = taskCreate(lcdTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
  analogCalibrate(1);
  analogCalibrate(2);


  driveEncLeft = (encoderInit(6, 7, false));
 driveEncRight = (encoderInit(9, 8, false));
  liftEnc = (encoderInit(4, 5, false));
}
