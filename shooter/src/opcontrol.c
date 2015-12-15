/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Copyright (c) 2011-2014, Purdue University ACM SIG BOTS.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Purdue University ACM SIG BOTS nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL PURDUE UNIVERSITY ACM SIG BOTS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Purdue Robotics OS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
#include "motorControl.c"
#include <Math.h>

const int MAX_FLYWHEEL_SPEED = 127;
const int MAX_PICKUP_SPEED = 127;

const int LEFT_FLY_OUT = 2;
const int LEFT_FLY_IN = 3;
const int RIGHT_FLY_OUT = 4;
const int RIGHT_FLY_IN = 5;

const int LEFT_BACK_DRIVE = 6;
const int RIGHT_BACK_DRIVE = 7;
const int LEFT_FRONT_DRIVE = 1;
const int RIGHT_FRONT_DRIVE = 10;

const int PICKUP_BELT = 8;

void operatorControl() {

	setMotorReversed(LEFT_FLY_OUT, true);
	setMotorReversed(RIGHT_FLY_IN, true);

	setMotorToRamp(LEFT_FLY_OUT, true);
	setMotorToRamp(LEFT_FLY_IN, true);
	setMotorToRamp(RIGHT_FLY_OUT, true);
	setMotorToRamp(RIGHT_FLY_IN, true);

	beginRampMotorsTask();

	while (1) {
		int button5U = joystickGetDigital(1,5,JOY_UP);
		int button6U = joystickGetDigital(1,6,JOY_UP);
		int button6D = joystickGetDigital(1,6,JOY_DOWN);

		int leftStick = joystickGetAnalog(1, 3);
		int rightStick = joystickGetAnalog(1, 2);

		setMotorSpeed(LEFT_BACK_DRIVE, leftStick);
		setMotorSpeed(LEFT_FRONT_DRIVE, leftStick / sqrt(2));

		setMotorSpeed(RIGHT_BACK_DRIVE, rightStick);
		setMotorSpeed(RIGHT_FRONT_DRIVE, rightStick / sqrt(2));

		if (button6U)
			setMotorSpeed(PICKUP_BELT, MAX_PICKUP_SPEED);
		else if (button6D)
			setMotorSpeed(PICKUP_BELT, -MAX_PICKUP_SPEED);
		else
			setMotorSpeed(PICKUP_BELT, 0);

		if(button5U){
			digitalWrite(1, LOW);
			rampMotorsUp(MAX_FLYWHEEL_SPEED);
		}
		else{
			rampMotorsDown(0);
		}

		delay(20);
	}
}
