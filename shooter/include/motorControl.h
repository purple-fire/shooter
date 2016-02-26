#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include "encoderTask.c"

//						     1  2  3  4  5  6  7  8  9  10 11 12
int motorReversed[] = 	   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int motorsToRamp[] =       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int motorLeftFlywheel[] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int motorRightFlywheel[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

bool rampingUp = false;
bool rampingDown = false;
int speedTarget = 0;
int lastSpeed = 0;

int rampUpDelay = 20;
int rampDownDelay = 20;

int flywheelTarget = 0;
int leftSpeed = 0;
int rightSpeed = 0;
int ENC_TOLERANCE = 25;

float pidIntegralLeft = 0;
float pidLastErrorLeft = 0;
float pidIntegralRight = 0;
float pidLastErrorRight = 0;

void setMotorToRamp(int channel, bool ramp);
void setMotorReversed(int channel, bool isReversed);
void setRampUpDelay(int delay);
void setRampDownDelay(int delay);

void beginRampMotorsTask();
void stopRampMotorsTask();

void setMotorSpeed(int channel, int speed);

void rampMotorsUp(int speed);
void rampMotorsDown(int speed);
void rampMotors();

void beginFlywheelControlTask();
void setLeftFlywheelMotor(int channel, bool flywheel);
void setRightFlywheelMotor(int channel, bool flywheel);
void setFlywheelTarget(int target);
void stopFlywheels();
void flywheelRamp();
void encPIDControllerLeft();
void encPIDControllerRight();

TaskHandle flywheelTask;
TaskHandle rampingTask;
TaskHandle leftPIDTask;
TaskHandle rightPIDTask;

#endif
