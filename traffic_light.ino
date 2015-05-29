/*
Adafruit Arduino - Lesson 3. RGB LED
*/
 
int r1 = 11;
int g1 = 12;
int b1 = 13;

int r2 = 8;
int g2 = 9;
int b2 = 10;

int r3 = 5;
int g3 = 6;
int b3 = 7;
 
//uncomment this line if using a Common Anode LED
//#define COMMON_ANODE
 
void setup()
{
  pinMode(r1, OUTPUT);
  pinMode(g1, OUTPUT);
  pinMode(b1, OUTPUT);  
  
  pinMode(r2, OUTPUT);
  pinMode(g2, OUTPUT);
  pinMode(b2, OUTPUT); 
  
  pinMode(r3, OUTPUT);
  pinMode(g3, OUTPUT);
  pinMode(b3, OUTPUT); 
}
 
void loop()
{
  setGreen(0, 255, 0);  // green
  delay(5000);
  turnOff();
  setYellow(200, 255, 0);  // yellow
  delay(2000); 
  turnOff();
  setRed(255, 0, 0);  // red
  delay(4000); 
  turnOff();
}
 
void setGreen(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  
  analogWrite(r1, red);
  analogWrite(g1, green);
  analogWrite(b1, blue);
}

void setYellow(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  
  analogWrite(r2, red);
  analogWrite(g2, green);
  analogWrite(b2, blue);  
}

void setRed(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  
  analogWrite(r3, red);
  analogWrite(g3, green);
  analogWrite(b3, blue);
}

void turnOff()
{
  analogWrite(r1, 0);
  analogWrite(g1, 0);
  analogWrite(b1, 0);
  analogWrite(r2, 0);
  analogWrite(g2, 0);
  analogWrite(b2, 0); 
  analogWrite(r3, 0);
  analogWrite(g3, 0);
  analogWrite(b3, 0);
}
