/*	    pins    	*/

// pins for the sound sensor
const int pinSS = A0;

// pins for the turret (servo)
const int pinTurret = 6; // PWM

// pins for the fire detection system
const int pinFDS1 = A1;
const int pinFDS2 = A2;
const int pinFDS3 = A3;

// pins for the fire extinguishing system
// see "ConfigurationsEFS.png" to know how to control it.
const int pinFES1 = 7; // goes to INA.
const int pinFES2 = 8; // goes to INB.




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
