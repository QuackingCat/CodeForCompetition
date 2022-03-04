/*
 * free pins (if used pls delete from here):
 * 0, 1, 2, 4, 6 (PWM), 13.
 *
 * terminology: (so much complicated acronyms)
 * US - Ultra sonic.
 * USR - Ultra sonic sensor on the right.
 * USL - Ultra sonic sensor on the left.
 * SS - sound sensor.
 * slave - the second arduino.
 * FDS - fire detection system.
 * FES - fire extinguishing system.
 */


/*	libraries	*/

#include <Wire.h>


/*	constants	*/

// pins for the US sensors
const int pinEchoUSR = A0;
const int pinTrigUSR = A1;
const int pinEchoUSL = A2;
const int pinTrigUSL = A3;

// pins for the sound sensor (PWM)
const int pinSS = 3;

// pins for the turret (PWM)
const int pinTurret = 5;

// pins for the fire detection system (PWM)
const int pinFDS1 = 9;
const int pinFDS2 = 10;
const int pinFDS3 = 11;

// pins for the fire extinguishing system
// see "ConfigurationsEFS.png" to know how to control it.
const int pinFES1 = 7; // goes to INA.
const int pinFES2 = 8; // goes to INB.

// I2C registers' addresses.
const int addressGyro = b1101001; // The address of the gyro's register (I2C shit).
const int addressSlave = b1000101; // The address of the slave's register (lol 69).


/*	main code	*/

void setup() {
	// US sensors
	pinMode(pinEchoUSR, INPUT);
	pinMode(pinTrigUSR, OUTPUT);
	pinMode(pinEchoUSL, INPUT);
	pinMode(pinEchoUSL, OUTPUT);
	
	// Sound sensor
	pinMode(pinSS, INPUT);
	
	// turret
	pinMode(pinTurret, OUTPUT);
	
	// FDS
	pinMode(pinFDS1, INPUT);
	pinMode(pinFDS2, INPUT);
	pinMode(pinFDS3, INPUT);
	
	// FES
	pinMode(pinFES1, INPUT);
	pinMode(pinFES2, INPUT);
	
	// I2C shit
	Wire.begin(b1); // turn on the I2C and set the address of this device's register.
}


void loop() {
	
}


// Return true if the robot is above white area and a sound of 3600Hz is being played for atleast 2 seconds.
boolean both()
{
	for (int i = 0; i < 4; i++){
		delay(500);
		if (!(checkWhiteArea() && checkSound()))
			return false;
	}
	
	return true;
}


void run()
{
	if(both())
	{
		//call the function to start the maze
	}
}


// returns the robot's rotation degree with respect to the initial angle.
int getDegree(){
	
}
