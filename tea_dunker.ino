// Tea Dunker
//
// A servo triggered by the ultrasonic output.
// If a cup is place close to the sensor, the tea will be dipped in the cup.
//
// @author: Aidan Melen
// @date: 06/08/2015

#include <Servo.h>

Servo servo;  // create servo object to control a servo

const int pingPin = 12;
const int servoPin = 13;
int pos = 60;    // variable to store the servo position

void setup()
{
  Serial.begin(9600);
  servo.attach(servoPin);  // attaches the servo on pin 11 to the servo object
  servo.write(pos);
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

void loop()
{
  servo.write(60);
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, inches;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  inches = microsecondsToInches(duration); // convert the time into a distance

  Serial.println(inches);
  Serial.print("in, ");

  if (inches < 2) // If a cup is detected
  {
    int count = 0;

    while (count < 180) // 3 minutes of tea dunking
    {
      //
      // servo motion UP
      //
      for (pos = 60; pos >= 0; pos -= 1)  // goes from 0 degrees to 60 degrees
      {
        servo.write(pos);// tell servo to go to position in variable 'pos'
        delay(30); // waits 30ms for the servo to reach the position
      }
      delay(100);
      count++;
      Serial.println(count);
      
      //
      // servo motion DOWN
      //
      for (pos = 0; pos <= 60; pos += 1) // goes from 60 degrees to 0 degrees
      {
        servo.write(pos);
        delay(30);
      }
      delay(100);
      count++;

      Serial.println(count);

      delay(30000); // allow time for user to remove mug and avoid instant re-triggering of ping sensor.
    }
  }
  else
  {
    pos = 60;
  }
}
