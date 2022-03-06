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
const int addressGyro = 0x68; // The address of the gyro's register (I2C shit).
const int addressSlave = 0x45; // The address of the slave's register (lol 69).


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
	
	Wire.beginTransmission(addressGyro); // start the transmission to the gyro.
	Wire.write(0x6B); // PWR_MGMT_1 register.
	Wire.write(0); // wakes up the sensor.
	Wire.endTransmission(true); // releasing the I2C bus.
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
int getDegrees(){
	Wire.beginTransmission(addressGyro); // start the transmission to the gyro.
	Wire.write(0x47); // start reading from this register.
	Wire.endTransmission(false); // commit without releasing the I2C bus.
	
	Wire.requestFrom(addressGyro, 2, true); // asking for two registers starting at "0x47".
	return (Wire.read() << 8) | Wire.read(); // reading registers "0x47" (GYRO_ZOUT_H) and "0x48" (GYRO_ZOUT_L). 
}


boolean checkSound()
{
	int sensorValue = analogRead(3);
	while (sensorValue < 3600)
	{
		sensorValue = analogRead(3);
	}
}


// checks if there is white area under the robot.
boolean checkWhiteArea() {
	return askSlave("isWhiteArea");
}


// sends string to the slave.
void sendToSlave(string str) {
	Wire.beginTransmission(addressSlave); // start the transmission to the gyro.
	Wire.write(str); // start reading from this register.
	Wire.endTransmission(true); // commit and releasing the I2C bus.
}


// ask for info fron the slave based on string.
int askSlave(string str) {
	sendToSlave(str);
	Wire.requestFrom(addressSlave, 1, true); // asking for one register.
	return Wire.read();
}
