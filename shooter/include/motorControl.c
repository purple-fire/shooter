#include "motorControl.h"

void setMotorToRamp(unsigned char channel, bool ramp){
	motorsToRamp[channel-1] = ramp;
}

void setMotorReversed(unsigned char channel, bool isReversed){
	motorReversed[channel-1] = isReversed;
}

void setRampUpDelay(int delay){
	rampUpDelay = delay;
}

void setRampDownDelay(int delay){
	rampDownDelay = delay;
}

void beginRampMotorsTask(){
	rampingTask = taskRunLoop(rampMotors, 10);
}

void stopRampMotorsTask(){
	taskSuspend(rampingTask);
}

void setMotorSpeed(unsigned char channel, int speed)
{
	if (motorReversed[channel-1] == 1)
		speed *= -1;

	motorSet(channel, speed);
}

void rampMotorsUp(int speed){
	rampingUp = true;
	speedTarget = speed;
}

void rampMotorsDown(int speed){
	rampingDown = true;
	speedTarget = speed;
}

void rampMotors(){
	if (rampingUp || rampingDown){
		if (speedTarget > lastSpeed){
			digitalWrite(1, LOW);
			for (int i = lastSpeed; i < speedTarget; i++){
				lastSpeed = i;
				for (int j = 0; j < 12; j++){
					if (motorsToRamp[j])
						setMotorSpeed(j, i);
				}

				delay(rampUpDelay);
			}
			rampingUp = false;
		}
		digitalWrite(1, HIGH);

		if (speedTarget < lastSpeed){
			for (int i = lastSpeed; i > speedTarget; i--){
				lastSpeed = i;

				for (int j = 0; j < 12; j++){
					if (motorsToRamp[j])
						setMotorSpeed(j, i);
				}

				delay(rampDownDelay);
			}
			rampingDown = false;
		}
	}
}
