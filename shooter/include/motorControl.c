// motorControl.c
#include "motorControl.h"

void setMotorToRamp(int channel, bool ramp) {
	motorsToRamp[channel - 1] = ramp;
}

void setMotorReversed(int channel, bool isReversed) {
	motorReversed[channel - 1] = isReversed;
}

void setRampUpDelay(int delay) {
	rampUpDelay = delay;
}

void setRampDownDelay(int delay) {
	rampDownDelay = delay;
}

void beginRampMotorsTask() {
	rampingTask = taskRunLoop(rampMotors, 10);
}

void stopRampMotorsTask() {
	taskSuspend(rampingTask);
}

void setMotorSpeed(int channel, int speed) {
	if (motorReversed[channel - 1] == 1)
		speed *= -1;

	motorSet(channel, speed);
}

void rampMotorsUp(int speed) {
	rampingUp = true;
	speedTarget = speed;
}

void rampMotorsDown(int speed) {
	rampingDown = true;
	speedTarget = speed;
}

void rampMotors() {
	if (rampingUp || rampingDown) {
		if (speedTarget > lastSpeed) {
			digitalWrite(1, LOW);
			for (int i = lastSpeed; i < speedTarget; i++) {
				lastSpeed = i;
				for (int j = 1; j <= 12; j++) {
					if (motorsToRamp[j - 1])
						setMotorSpeed(j, i);
				}

				delay(rampUpDelay);
			}
			rampingUp = false;
		}

		if (speedTarget < lastSpeed) {
			for (int i = lastSpeed; i > speedTarget; i--) {
				lastSpeed = i;

				for (int j = 1; j <= 12; j++) {
					if (motorsToRamp[j - 1])
						setMotorSpeed(j, i);
				}

				delay(rampDownDelay);
			}
			rampingDown = false;
		}
	}
}

void setLeftFlywheelMotor(int channel, bool flywheel) {
	motorLeftFlywheel[channel - 1] = flywheel;
}

void setRightFlywheelMotor(int channel, bool flywheel) {
	motorRightFlywheel[channel - 1] = flywheel;
}

void setFlywheelTarget(int target) {
	flywheelTarget = target;
}

void stopFlywheels() {
	flywheelTarget = 0;
}

void beginFlywheelControlTask() {
	//flywheelTask = taskRunLoop(flywheelRamp, 100);
	leftPIDTask = taskRunLoop(encPIDControllerLeft, 250);
	rightPIDTask = taskRunLoop(encPIDControllerRight, 250);
}
void flywheelRamp() {
	int leftEncoder = getLeftSpeed();
	int rightEncoder = getRightSpeed();

	if (rightEncoder < flywheelTarget
			&& abs(flywheelTarget - rightEncoder) > ENC_TOLERANCE) {
		if (++rightSpeed > 127)
			rightSpeed = 127;

		for (int i = 0; i < 12; i++)
			if (motorRightFlywheel[i])
				setMotorSpeed(i+1, rightSpeed);
	} else if (rightEncoder > flywheelTarget
			&& abs(flywheelTarget - rightEncoder) > ENC_TOLERANCE) {
		if (--rightSpeed < 0)
			rightSpeed = 0;

		for (int i = 0; i < 12; i++)
			if (motorRightFlywheel[i])
				setMotorSpeed(i+1, rightSpeed);
	} else {
		if (flywheelTarget == 0)
			rightSpeed = 0;
		for (int i = 0; i < 12; i++)
			if (motorRightFlywheel[i]) {
				setMotorSpeed(i+1, rightSpeed);
			}
	}

	if (leftEncoder < flywheelTarget
			&& abs(flywheelTarget - leftEncoder) > ENC_TOLERANCE) {
		if (++leftSpeed > 127)
			leftSpeed = 127;

		for (int i = 0; i < 12; i++)
			if (motorLeftFlywheel[i])
				setMotorSpeed(i+1, leftSpeed);
	} else if (leftEncoder > flywheelTarget
			&& abs(flywheelTarget - leftEncoder) > ENC_TOLERANCE) {
		if (--leftSpeed < 0)
			leftSpeed = 0;

		for (int i = 0; i < 12; i++)
			if (motorLeftFlywheel[i])
				setMotorSpeed(i+1, leftSpeed);
	} else {
		if (flywheelTarget == 0)
			leftSpeed = 0;
		for (int i = 0; i < 12; i++)
			if (motorLeftFlywheel[i])
				setMotorSpeed(i+1, leftSpeed);
	}

}

void encPIDControllerLeft() {
	float pid_Ki = 0;//.00125/3;
	float pid_Kp = .00750;
	float pid_Kd = .00125;
	float PID_INTEGRAL_LIMIT = 3000;
	int MOTOR_MAXSPEED = 127;
	int MOTOR_MINSPEED = -127;

	float pidSensorCurrentValueLeft;
	float pidError;
	float pidDerivative;
	float pidDrive;
	int PID_SENSOR_SCALE = 1;


	pidSensorCurrentValueLeft = getLeftSpeed() * PID_SENSOR_SCALE;

	// calculate error
	pidError = flywheelTarget - pidSensorCurrentValueLeft;
	printf("The PID Error is %f\n\r", pidError);

	if (abs(pidError) > ENC_TOLERANCE) {
		// integral - if Ki is not 0
		if (pid_Ki != 0) {
			// If we are inside controlable window then integrate the error
			if (abs(pidError) < PID_INTEGRAL_LIMIT) {
				pidIntegralLeft = pidIntegralLeft + pidError;
			} else
				pidIntegralLeft = 0;
		} else
			pidIntegralLeft = 0;
		//printf("PID Integral: %f", pidIntegral);
		// calculate the derivative
		pidDerivative = pidError - pidLastErrorLeft;
		pidLastErrorLeft = pidError;

		// calculate drive
		pidDrive = (pid_Kp * pidError) + (pid_Ki * pidIntegralLeft)
				+ (pid_Kd * pidDerivative);
		printf("LPIDDRIVE: %f\n\r", pidDrive);

		// send to motor
		leftSpeed += pidDrive;

		// limit drive
		if (leftSpeed > MOTOR_MAXSPEED)
			leftSpeed = MOTOR_MAXSPEED;
		if (leftSpeed < MOTOR_MINSPEED)
			leftSpeed = MOTOR_MINSPEED;


	}
	else if (pidError > 0 || pidError < ENC_TOLERANCE){
		leftSpeed+=0;
	}
	else{
		leftSpeed += 0;
	}

	for (int i = 0; i < 12; i++)
		if (motorLeftFlywheel[i]) {
			setMotorSpeed(i+1, leftSpeed);
		}
}

void encPIDControllerRight() {
	float pid_Ki = 0;//.00125/3;
	float pid_Kp = .00750;
	float pid_Kd = .00125;
	float PID_INTEGRAL_LIMIT = 3000;
	int MOTOR_MAXSPEED = 127;
	int MOTOR_MINSPEED = -127;

	float pidSensorCurrentValueLeft;
	float pidError;
	float pidDerivative;
	float pidDrive;
	int PID_SENSOR_SCALE = 1;


	pidSensorCurrentValueLeft = getRightSpeed() * PID_SENSOR_SCALE;

	// calculate error
	pidError = flywheelTarget - pidSensorCurrentValueLeft;
	printf("The PID Error is %f\n\r", pidError);

	if (abs(pidError) > ENC_TOLERANCE) {
		// integral - if Ki is not 0
		if (pid_Ki != 0) {
			// If we are inside controlable window then integrate the error
			if (abs(pidError) < PID_INTEGRAL_LIMIT) {
				pidIntegralRight = pidIntegralRight + pidError;
			} else
				pidIntegralRight = 0;
		} else
			pidIntegralRight = 0;
		//printf("PID Integral: %f", pidIntegral);
		// calculate the derivative
		pidDerivative = pidError - pidLastErrorRight;
		pidLastErrorRight = pidError;

		// calculate drive
		pidDrive = (pid_Kp * pidError) + (pid_Ki * pidIntegralRight)
				+ (pid_Kd * pidDerivative);
		printf("LPIDDRIVE: %f\n\r", pidDrive);

		// send to motor
		rightSpeed += pidDrive;

		// limit drive
		if (rightSpeed > MOTOR_MAXSPEED)
			rightSpeed = MOTOR_MAXSPEED;
		if (rightSpeed < MOTOR_MINSPEED)
			rightSpeed = MOTOR_MINSPEED;


	}
	else if (pidError > 0 || pidError < ENC_TOLERANCE){
		rightSpeed+=0;
	}
	else{
		rightSpeed += 0;
	}

	for (int i = 0; i < 12; i++)
		if (motorRightFlywheel[i]) {
			setMotorSpeed(i+1, rightSpeed);
		}
}
