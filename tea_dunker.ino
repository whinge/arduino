/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup()
{
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop()
{
  int count = 0; // total seconds
  while (count < 60)
  {
    for (pos = 0; pos <= 60; pos += 1) // goes from 0 degrees to 60 degrees
    { // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(30);                       // waits 30ms for the servo to reach the position
    }
    delay(1000);
    for (pos = 60; pos >= 0; pos -= 1) // goes from 60 degrees to 0 degrees
    {
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(30);                       // waits 30ms for the servo to reach the position
    }
    delay(1000);
  }
  count = count-1;
}
