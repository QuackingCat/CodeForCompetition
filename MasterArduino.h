/*	    pins    	*/

// pins for the sound sensor
int pinSS = A0;

// pins for the turret (servo)
int pinTurret = 6; // PWM pin

// pins for the fire detection system
int pinFDS1 = A1; // IR sensor
int pinFDS2 = A2; // IR sensor
int pinFDS3 = A3; // IR sensor

// pins for the fire extinguishing system
// see "ConfigurationsEFS.png" to know how to control it.
int pinFES1 = 7; // goes to INA.
int pinFES2 = 8; // goes to INB.




/*      functions     */

// returns the robot's rotation degree with respect to the initial angle.
int getDegrees();

// checks if there is white area under the robot.
boolean checkWhiteArea();


// move forward
void moveForward();

// move backward
void moveBackward();

// move right
void moveRight();

// move left
void moveLeft();

// stop moving
void stopMoving();


// returns the front distnance.
int getFrontDis();

// returns the back distnance.
int getBackDis();

// returns the right distnance.
int getRightDis();

// returns the left distnance.
int getLeftDis();
