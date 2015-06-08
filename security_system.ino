/*
 * An Embedded Systems group project.
 *
 * The PIR Sensor is an easy to use digital infrared motion sensor module.
 * The sensor's output pin goes to HIGH if motion is present; else it will go LOW.
 *
 * @author: Aidan Melen, Conrad Lieness, Grady Mellin
 * @date: 05/14/2015
 */

boolean first = true;

int ledPin = 13;
int pirPin = 12;    // the digital pin connected to the PIR sensor's output
int buzzerPin = 11;

int calibrationTime = 10; // the time we give the sensor to calibrate (10-60 secs according to the datasheet)
boolean alarm = false;


// initialize keypad variables

byte h = 0, v = 0; //variables used in for loops

const unsigned long period = 50; //little period used to prevent error

unsigned long kdelay = 0;      // variable used in non-blocking delay

const byte rows = 4;           //number of rows of keypad

const byte columns = 4;          //number of columnss of keypad

const byte Output[rows] = {5, 4, 3, 2}; //array of pins used as output for rows of keypad

const byte Input[columns] = {9, 8, 7, 6}; //array of pins used as input for columnss of keypad


byte keypad() { // function used to detect which button is used

  static bool no_press_flag = 0; //static flag used to ensure no button is pressed

  for (byte x = 0; x < columns; x++) { // for loop used to read all inputs of keypad to ensure no button is pressed

    if (digitalRead(Input[x]) == HIGH); //read evry input if high continue else break;
    else
      break;

    if (x == (columns - 1)) { //if no button is pressed
      no_press_flag = 1;
      h = 0;
      v = 0;
    }
  }

  if (no_press_flag == 1) { //if no button is pressed

    for (byte r = 0; r < rows; r++) { //for loop used to make all output as low
      digitalWrite(Output[r], LOW);
    }

    for (h = 0; h < columns; h++) { // for loop to check if one of inputs is low
      if (digitalRead(Input[h]) == HIGH) //if specific input is remain high (no press on it) continue
        continue;

      else {   //if one of inputs is low
        for (v = 0; v < rows; v++) { //for loop used to specify the number of row

          digitalWrite(Output[v], HIGH);  //make specified output as HIGH

          if (digitalRead(Input[h]) == HIGH) { //if the input that selected from first sor loop is change to high
            no_press_flag = 0;              //reset the no press flag;

            for (byte w = 0; w < rows; w++) { // make all outputs as low
              digitalWrite(Output[w], LOW);
            }

            return v * 4 + h; //return number of button

          }
        }
      }
    }
  }

  return 50;

}


/*
  Setup function
  **/
void setup() {
  Serial.begin(9600);

  // setup keypad
  for (byte i = 0; i < rows; i++) { //for loop used to make pin mode of outputs as output
    pinMode(Output[i], OUTPUT);
  }

  for (byte s = 0; s < columns; s++) { //for loop used to makk pin mode of inputs as inputpullup
    pinMode(Input[s], INPUT_PULLUP);
  }

  Serial.begin(9600); //to use serial monitor we set the buad rate

  // setup other peripherals
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // initialize them OFF
  digitalWrite(pirPin, LOW);
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);


  //give the sensor some time to calibrate
  Serial.println("CALIBRATING SENSOR");
  for (int i = calibrationTime; i > 0; i--) {
    Serial.println(i);
    digitalWrite(ledPin, HIGH); // led ON
    delay(500);
    digitalWrite(ledPin, LOW); // led OFF
    delay(500);
  }
  Serial.println("SENSOR ACTIVE");
}

void alarmCheck() {
    while (alarm) {
      // check keypad for press
      if (millis() - kdelay > period) { //used to make non-blocking delay
        kdelay = millis(); //capture time from millis function
        switch (keypad()) { //switch used to specify which button
          case 0:
            Serial.println("D");
            break;

          case 1:
            Serial.println("#");
            break;

          case 2:
            Serial.println(0);
            break;

          case 3:
            Serial.println("*");
            break;

          case 4:
            Serial.println("C");
            alarm = false;
            break;

          case 5:
            Serial.println(9);
            break;

          case 6:
            Serial.println(8);
            break;

          case 7:
            Serial.println("F2");
            break;

          case 8:
            Serial.println(6);
            break;

          case 9:
            Serial.println(5);
            break;

          case 10:
            Serial.println(5);
            break;

          case 11:
            Serial.println(4);
            break;

          case 12:
            Serial.println("A");
            break;

          case 13:
            Serial.println(3);
            break;

          case 14:
            Serial.println(2);
            break;

          case 15:
            Serial.println(1);
            break;

          default:;
        }
        
        // 1st second of press
        analogWrite(buzzerPin, 25);
        digitalWrite(ledPin, HIGH);
        delay(500);
        analogWrite(buzzerPin, LOW);
        digitalWrite(ledPin, LOW);
        delay(500);
        
        // 2nd second of press
        analogWrite(buzzerPin, 25);
        digitalWrite(ledPin, HIGH);
        delay(500);
        analogWrite(buzzerPin, LOW);
        digitalWrite(ledPin, LOW);
        delay(500);
        
        // 3rd second of press
        analogWrite(buzzerPin, 25);
        digitalWrite(ledPin, HIGH);
        delay(500);
        analogWrite(buzzerPin, LOW);
        digitalWrite(ledPin, LOW);
        delay(500);
      }
    }
}
  

/*
 Loop function
 **/
void loop() {

  if (digitalRead(pirPin) == HIGH) { // object detected

    alarm = true; // alarm was triggered

    if (first) {
      Serial.println("---------------");
      Serial.println("MOTION DETECTED");
      Serial.println("---------------");
      Serial.println();
      alarmCheck();
      first = false;
    }
    
  } else { // no objects detected
    digitalWrite(ledPin, LOW);
    analogWrite(buzzerPin, LOW);
    delay(500);
    first = true;
  }

}
