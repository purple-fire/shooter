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

// int MAX_FLYWHEEL_SPEED = 69;
int MAX_PICKUP_SPEED = 127;
int MAX_RELEASE_SPEED = 127;

int LEFT_FLY_OUT = 6;
int LEFT_FLY_IN = 7;
int RIGHT_FLY_OUT = 4;
int RIGHT_FLY_IN = 5;

int LEFT_BACK_DRIVE = 2;
int RIGHT_BACK_DRIVE = 3;
int LEFT_FRONT_DRIVE = 10;
int RIGHT_FRONT_DRIVE = 1;

int PICKUP_BELT = 8;
int RAMP_RELEASE = 9;

int modFlywheelSpeed = 0;
Encoder LFlywheel;
Encoder RFlywheel;

void intializeIO(){
	pinMode(1, INPUT);
	pinMode(2, INPUT);
	pinMode(3, INPUT);
	pinMode(4, INPUT);
}

void initialize(){
	LFlywheel = encoderInit(5, 6, false);
	RFlywheel = encoderInit(7, 8, true);

	modFlywheelSpeed = 1250;

	setMotorReversed(LEFT_FLY_IN, true);
	setMotorReversed(RIGHT_FLY_OUT, true);
	setMotorReversed(RIGHT_FRONT_DRIVE, true);
	setMotorReversed(LEFT_BACK_DRIVE, true);

	setLeftFlywheelMotor(LEFT_FLY_IN, true);
	setLeftFlywheelMotor(LEFT_FLY_OUT, true);
	setRightFlywheelMotor(RIGHT_FLY_IN, true);
	setRightFlywheelMotor(RIGHT_FLY_OUT, true);
}

void driving(){
	setupEncoder(LFlywheel, 1);
	setupEncoder(RFlywheel, 2);

	beginEncoderTask();
	beginFlywheelControlTask();
	while (1) {
		int button5U = joystickGetDigital(1,5,JOY_UP);
		int button6U = joystickGetDigital(1,6,JOY_UP);
		int button6D = joystickGetDigital(1,6,JOY_DOWN);
		int button5D = joystickGetDigital(1, 5, JOY_DOWN);

		int button7U = joystickGetDigital(1,7,JOY_UP);
		int button7D = joystickGetDigital(1,7,JOY_DOWN);
		int button7L = joystickGetDigital(1,7,JOY_LEFT);
		int button7R = joystickGetDigital(1,7,JOY_RIGHT);

		int leftStick = joystickGetAnalog(1, 3);
		int rightStick = joystickGetAnalog(1, 1);

		setMotorSpeed(LEFT_BACK_DRIVE, leftStick);
		setMotorSpeed(LEFT_FRONT_DRIVE, rightStick);

		setMotorSpeed(RIGHT_BACK_DRIVE, leftStick);
		setMotorSpeed(RIGHT_FRONT_DRIVE, rightStick);

		if (button6U)
			setMotorSpeed(PICKUP_BELT, -MAX_PICKUP_SPEED);
		else if (button6D)
			setMotorSpeed(PICKUP_BELT, MAX_PICKUP_SPEED);
		else
			setMotorSpeed(PICKUP_BELT, 0);

		if (button7U)
			modFlywheelSpeed = 1400;
		if (button7D)
			modFlywheelSpeed = 1100;
		if (button7L)
			modFlywheelSpeed = 1200;
		if (button7R)
			modFlywheelSpeed = 1300;

		if(button5U)
			setFlywheelTarget(modFlywheelSpeed);
		else
			stopFlywheels();

		if (button5D)
			setMotorSpeed(RAMP_RELEASE, MAX_RELEASE_SPEED);
		else
			setMotorSpeed(RAMP_RELEASE, 0);

		delay(10);
	}
}

void autonomous(){
	setupEncoder(LFlywheel, 1);
	setupEncoder(RFlywheel, 2);

	beginEncoderTask();
	beginFlywheelControlTask();

	setFlywheelTarget(1300);
	setMotorSpeed(PICKUP_BELT, MAX_PICKUP_SPEED);
	delay(20000);
}
void skills(){
	setFlywheelTarget(1300);
	setMotorSpeed(PICKUP_BELT, MAX_PICKUP_SPEED);
}

void operatorControl() {
	driving();
}
