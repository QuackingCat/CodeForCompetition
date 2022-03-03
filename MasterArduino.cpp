// constants
const int pinIRa = A0; // "AO" pin of the light sensor (Analog).

// variables
int inlValueLightSensor = 0; // The value used to determine when there is a white area.


void setup() {
  pinMode(pinIRa,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

}

// Return true if both the robot is above white area and a sound of 3600Hz is bieng played.
public boolean both()
{
  return checkWhiteArea() // checks if the robot is above white area.
      && checkSound(); // checks if there is a sound of 3600Hz.
}

int checkWhiteArea(){
  if ()
  return analogRead(pinIRa);
}

public void run()
{
  if(both() == true)
  {
    //call the function to start the maze
  }
}

