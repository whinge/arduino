// Determine BAC levels with the MQ-3 Alcohol Sensor.
//
// The MQ3 sensor produces an analog output signal between 0 and 1023.
// BAC is expressed as the weight of ethanol, measured in grams per 210 liters of breath.
// The graphs in the PDFs give breath alcohol in terms of mg/L.
// To convert mg/L to BAC, multiply by 210 and divide by 1000, or in other words, multiply by 0.21.
//
// For example, if the analog output from our MQ3 is reading 400, we divide 400 by 1023 (the highest analog value) in order to get the ratio or percentage of alcohol on the breath.
// Then we determine that 0.4 or 40% alcohol on the breath will yield 0.4 * 0.21 = 0.084, which is slightly more than the legal limit in in most states of 0.08.
//
// @author: Aidan Melen
// @date: 06/08/2015

const int mq3Pin = 0; // The output from the MQ3 alcohol sensor goes into analog pin A0 of the arduino
const int buzzerPin = 13; // buzzer goes into analog pin A1 of the arduino.
const int frequency = 450;

// The RGD-LED connects to digital pins D10-D12 on the arduino
const int redPin = 12;
const int grePin = 11;
const int bluPin = 10;

int value; // Holds the analog value from the MQ3 sensor
double percentage; // stores the percentage of alcohol in blood
double bac; // stores the calculated Blood Alcohol Content value
double initBAC; // stores initial BAC. Used to compensate for humidity and tempurature

// Used for timing a 5 second long blow form the user
int startTime;
int currentTime;

boolean first = true; // compensate for humidity


//
// Setup function
//
void setup() {
  Serial.begin(9600);// sets the baud rate

  // sets the RGB-LED's as an output for the arduino
  pinMode(redPin, OUTPUT);
  pinMode(grePin, OUTPUT);
  pinMode(bluPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}


//
// Red led function. Sensor is calibrating or recalibrating.
//
void redLed()
{
  int i = 30; // 30 seconds
  while (i > 0) {
    digitalWrite(redPin, 255); // turn ON
    digitalWrite(grePin, 0);
    digitalWrite(bluPin, 0);
    delay(500); // delay 0.5 seconds
    digitalWrite(redPin, LOW); // turn OFF
    digitalWrite(grePin, LOW);
    digitalWrite(bluPin, LOW);
    delay(500);
    Serial.println(i);
    i--;
  }
}


//
// Green led  ON function. User is ready.
//
void greenLed() {
  delay(250);
  analogWrite(buzzerPin, frequency);
  digitalWrite(redPin, 0); // turn ON
  digitalWrite(grePin, 255);
  digitalWrite(bluPin, 0);
  delay(250);
  analogWrite(buzzerPin, LOW);
  digitalWrite(redPin, LOW); // turn OFF
  digitalWrite(grePin, LOW);
  digitalWrite(bluPin, LOW);
  delay(250);
  analogWrite(buzzerPin, frequency);
  digitalWrite(redPin, 0);
  digitalWrite(grePin, 255);
  digitalWrite(bluPin, 0);
  delay(250);
  analogWrite(buzzerPin, LOW);
  digitalWrite(redPin, LOW);
  digitalWrite(grePin, LOW);
  digitalWrite(bluPin, LOW);
  delay(500);
}


//
// Blue led function. User is blowing.
//
void blueLedON()
{
  digitalWrite(redPin, 0); // turn ON
  digitalWrite(grePin, 0);
  digitalWrite(bluPin, 255);
}


//
// Blue led function. User is finished blowing.
//
void blueLedOFF()
{
  analogWrite(buzzerPin, frequency);
  delay(250);
  digitalWrite(redPin, LOW); // turn OFF
  digitalWrite(grePin, LOW);
  digitalWrite(bluPin, LOW);
  analogWrite(buzzerPin, LOW);
}

//
// Record MQ3 sensor for 5 seconds.
//
double readBAC() {
  while (currentTime - startTime < 5000) {

    value = analogRead(mq3Pin);//reads the analaog value from the alcohol sensor's AOUT pin

    percentage = value / 1023.0;
    bac = percentage * 0.21; // calculate BAC

    currentTime = millis();
  } // end while
  return bac;
}




//
// Loop function
//
void loop()
{
  //
  // Get a reading without any alcohol present. 
  // Use this reading to account for the MQ3 humidity and tempurature error. 
  //
  if (first) {
    Serial.println("Initalize \n");
    initBAC = readBAC();
    first = false;
    
  } else {
    Serial.println("Calibration \n");
    redLed();

    Serial.println("User Ready \n");
    greenLed();

    Serial.println("Blow \n");
    blueLedON();



    startTime = millis(); // begin timer
    currentTime = startTime;

    bac = readBAC();
    bac = bac - initBAC;

    //
    // Print output String and assume that any BAC reading below 0.02 is really 0 because the sensor doesn't always drop to absolute 0 when testing.
    //
    if (bac > 0.01) {
      Serial.println(String("BAC Result: ") + bac + "\n");
      Serial.println();
    } else {
      Serial.println("No alcohol was detected \n");
      Serial.println();
    }
    blueLedOFF(); // User finished blowing
  }
}
