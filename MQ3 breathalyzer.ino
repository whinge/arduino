// Determine BAC levels with the MQ-3 Alcohol Sensor.
//
// The MQ3 sensor produces an analog output signal between 0 and 1023.
// BAC is expressed as the weight of ethanol, measured in grams per 210 liters of breath.
// The graphs in the PDFs give breath alcohol in terms of mg/L.
// To convert mg/L to BAC, multiply by 210 and divide by 1000, or in other words, multiply by 0.21.
//
// For example, if the analog output from our MQ3 is reading 400, we divide 400 by 1023 (the highest analog value) in order to get the ratio or percentage of alcohol on the breath.
// Then we determine that 0.4 or 40% alcohol on the breath will yield 0.4 * 0.21 = 0.084, which is slightly more than the legal alimit in in most states of 0.08.
//
// @author: Aidan Melen
// @date: 06/08/2015

#include <LiquidCrystal.h>

const int mq3Pin = A0; // The output from the MQ3 alcohol sensor goes into analog pin A0 of the arduino
const int buttonPin = 2;
const int buzzerPin = 3; // buzzer goes into analog pin A1 of the arduino.
const int frequency = 450;
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int value; // Holds the analog value from the MQ3 sensor
double percentage; // stores the percentage of alcohol in blood
double bac; // stores the calculated Blood Alcohol Content value

// Used for timing a 5 second long blow form the user
int startTime;
int currentTime;

boolean first = true; // compensate for humidity

//
// Setup function
//
void setup() {
  Serial.begin(9600); // sets the baud rate
  lcd.begin(16, 4);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void buttonWait() {
  while (digitalRead(buttonPin) == HIGH) { // while button not pressed
    Serial.print("");
  }
}

void playSound() {
  analogWrite(buzzerPin, frequency);
  delay(250);
  analogWrite(buzzerPin, LOW);
}

void flashScreen() {
  delay(1000);
  lcd.setCursor(6, 1);
  lcd.print("ARDUINO");

  lcd.setCursor(8, 2);
  lcd.print("BREATHALYZER");
}

void calibrationScreen() {
  lcd.setCursor(1, 0);
  lcd.print("CALIBRATING SENSOR");
  Serial.println("CALIBRATING SENSOR \n");

  for (int i = 30; i > 0; i--) { // 30 seconds
    delay(1000);
    if (i >= 10) {
      lcd.setCursor(13, 2);
      lcd.print(i);
    } else { // reformat single digit values
      lcd.setCursor(13, 2);
      lcd.print("   ");
      lcd.setCursor(14, 2);
      lcd.print(i);
    }
  }
}

void buttonScreen() {
  lcd.setCursor(4, 0);
  lcd.print("SENSOR READY");
  Serial.print("SENSOR READY \n");
  lcd.setCursor(6, 2);
  lcd.print("PRESS THE BUTTON");
  lcd.setCursor(8, 3);
  lcd.print("TO CONTINUE");

  buttonWait();
  
  playSound();
}


//
// Record MQ3 sensor for 5 seconds.
//
double readBAC() {
  lcd.setCursor(2, 1);
  lcd.print("BLOW INTO SENSOR");
  startTime = millis(); // begin timer
  currentTime = startTime;

  while (currentTime - startTime < 5000) {

    value = analogRead(mq3Pin); //reads the analaog value from the alcohol sensor's AOUT pin

    percentage = value / 1023.0;
    bac = percentage * 0.21; // calculate BAC

    currentTime = millis();
  } // end while
  
  playSound();
  
  return bac;
}


//
// Loop function
//
void loop() {
  if (first) {
    flashScreen();
    delay(2000);
    lcd.clear();
    first = false;
  }

  calibrationScreen();
  lcd.clear();

  buttonScreen();
  lcd.clear();

  bac = readBAC();
  lcd.clear();

  //
  // Print output String and assume that any BAC reading below 0.01 is really 0 because the sensor doesn't always drop to absolute 0 when testing.
  //
  if (bac > 0.01) {
    lcd.setCursor(2, 0);
    lcd.print(String("BAC RESULT: ") + bac);
    lcd.setCursor(6, 2);
    lcd.print("PRESS THE BUTTON");
    lcd.setCursor(8, 3);
    lcd.print("TO CONTINUE");
    buttonWait();

  } else {
    lcd.setCursor(1, 0);
    lcd.print("NO ALCOHOL PRESENT");
    lcd.setCursor(6, 2);
    lcd.print("PRESS THE BUTTON");
    lcd.setCursor(8, 3);
    lcd.print("TO CONTINUE");
    buttonWait();
  }
  lcd.clear();
}
