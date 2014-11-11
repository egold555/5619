#pragma config(Sensor, S1,     ,               sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "../Libraries/I2C.h"
#include "../autonomous/IRfunctions - original.c"

const int threshold = 18;
int State = 0;
int joystick1[4];
float power[2];


void updateJoystick() {
	joystick1[0] = joystick.joy1_y1;
	joystick1[1] = joystick.joy1_x2;
	joystick1[2] = joystick.joy1_y2;
	joystick1[3] = joystick.joy1_x1;
	for(int i = 0; i <= 4; i++) {
		if(joystick1[i] >= 100)
			joystick1[i] = 99;
	}
}



float angleX(int x, int y) {
	return atan(x/y);
}

float angleY(int x, int y) {
	return atan(y/x);
}

void calcPow1(int x, int y, int goal, float* power) {
	int angle = angleY(x, y) + 0;
	calcPower(angle, goal, power);
}
void calcPow2(int x, int y, int goal, float* power) {
	int angle = angleX(x, y) + 90;
	calcPower(angle, goal, power);
}
void calcPow3(int x, int y, int goal, float* power) {
	int angle = angleY(x, y) + 180;
	calcPower(angle, goal, power);
}
void calcPow4(int x, int y, int goal, float* power) {
	int angle = angleX(x, y) + 270;
	calcPower(angle, goal, power);
}

void doStuff() {
	updateJoystick();
	if(joystick1[0] > threshold)
		State = 1;
	else if(joystick1[0] > threshold && joystick1[1] > threshold)
		State = 2;
	else if(joystick1[1] > threshold)
		State = 3;
	else if(joystick1[1] > threshold && joystick1[2] > threshold)
		State = 4;
	else if(joystick1[2] > threshold)
		State = 5;
	else if(joystick1[2] > threshold && joystick1[3] > threshold)
		State = 6;
	else if(joystick1[3] > threshold)
		State = 7;
	else if(joystick1[3] > threshold && joystick1[0] > threshold)
		State = 8;
	else
		State = 0;
}

task main()
{
	//while(true) {
	//	getJoystickSettings(joystick);
	//	doStuff();
	//}

	waitForStart();

	while(true)
	{
		// state machine loop
		doStuff();
		getJoystickSettings(joystick);

		switch(State)
		{
			case 0:
				I2C_SetMotorSpeed(S1, 1, 1, 0);
				I2C_SetMotorSpeed(S1, 1, 2, 0);
				I2C_SetMotorSpeed(S1, 2, 2, 0);
				break;
			case 1:
				I2C_SetMotorSpeed(S1, 1, 1, joystick1[0]);
				I2C_SetMotorSpeed(S1, 1, 2, joystick1[0]);
				break;
			case 2:
				if(joystick1[0] >= joystick1[1]) {
					calcPow1(joystick1[1], joystick1[0], joystick1[0], power);
				}
				else if( joystick1[0] < joystick1[1]) {
					calcPow1(joystick1[1], joystick1[0], joystick1[1], power);
				}
				I2C_SetMotorSpeed(S1, 1, 1, power[1]);
				I2C_SetMotorSpeed(S1, 1, 2, power[1]);
				I2C_SetMotorSpeed(S1, 2, 2, power[0]);
				break;
			case 3:
				I2C_SetMotorSpeed(S1, 2, 2, joystick1[1]);
				break;
			case 4:
				if(joystick1[1] >= joystick1[2]) {
					calcPow4(joystick1[1], joystick1[0], joystick1[1], power);
				}
				else if( joystick1[1] < joystick1[2]) {
					calcPow4(joystick1[1], joystick1[0], joystick1[2], power);
				}
				I2C_SetMotorSpeed(S1, 1, 1, power[1]);
				I2C_SetMotorSpeed(S1, 1, 2, power[1]);
				I2C_SetMotorSpeed(S1, 2, 2, power[0]);
				break;
			case 5:
				I2C_SetMotorSpeed(S1, 1, 1, -joystick1[2]);
				I2C_SetMotorSpeed(S1, 1, 2, -joystick1[2]);
				break;
			case 6:
				if(joystick1[2] >= joystick1[3]) {
					calcPow3(joystick1[3], joystick1[2], joystick1[2], power);
				}
				else if( joystick1[2] < joystick1[3]) {
					calcPow3(joystick1[3], joystick1[2], joystick1[3], power);
				}
				I2C_SetMotorSpeed(S1, 1, 1, power[1]);
				I2C_SetMotorSpeed(S1, 1, 2, power[1]);
				I2C_SetMotorSpeed(S1, 2, 2, power[0]);
				break;
			case 7:
				I2C_SetMotorSpeed(S1, 2, 2, -joystick1[3]);
				break;
			case 8:
				if(joystick1[3] >= joystick1[0]) {
					calcPow2(joystick1[3], joystick1[0], joystick1[3], power);
				}
				else if( joystick1[3] < joystick1[0]) {
					calcPow2(joystick1[3], joystick1[0], joystick1[0], power);
				}
				I2C_SetMotorSpeed(S1, 1, 1, power[1]);
				I2C_SetMotorSpeed(S1, 1, 2, power[1]);
				I2C_SetMotorSpeed(S1, 2, 2, power[0]);
				break;
			default:
				writeDebugStreamLine("FATAL ERROR ENCOUNTERED");
				writeDebugStreamLine("The state machine has encountered an unknown state. Something is SERIOUSLY wrong!");
				writeDebugStreamLine("Aborting program.");
				// TODO: stop all motors
				StopAllTasks();
				break;
		}
	}

}