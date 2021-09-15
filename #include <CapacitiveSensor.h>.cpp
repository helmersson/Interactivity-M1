#include <CapacitiveSensor.h>

enum ledStates {INACTIVE, CLOSEPROXIMITY, TOUCH, ANGRY}; // Here we make nicknames for the different states our program supports.
enum ledStates ledState; // We define 'ledState' as type ledStates'
enum ledStates previousLedState = ledState;

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;

//controls the brightness sinwave
int brightness ; // our main variable for setting the brightness of the LED
int sinDuration ;
int sinAmp ;
int sinOffset ;

//creates the sinwave
int sinewave(float duration, float amplitude, int offset) {
  // Generate a sine oscillation, return a number.
  // In case you are using this for analogWrite, make sure the amplitude does not exceed 256
  float period = millis() / duration; // Duration in ms determines the wavelength.
  float midpoint = amplitude / 2; // set the midpoint of the wave at half the amplitude so there are no negative numbers
  int value = midpoint + midpoint * sin ( period * 2.0 * PI );
  value = value + offset; //offset allows you to move the wave up and down on the Y-axis. Should not exceed the value of amplitude to prevent clipping.
  return value;
}


CapacitiveSensor cs_4_2 = CapacitiveSensor(4, 2);

//SMOOTHING
const int numReadings = 10;

int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;

void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
}

void loop() {
  //  compose();
  currentMillis = millis();
  //unsigned long total = smooth_sensor(cs_4_2.capacitiveSensor(30));
  unsigned long total = cs_4_2.capacitiveSensor(30);
  //  total = constrain(total, 0, 35000);

  Serial.println(millis() - currentMillis);
  Serial.println("\t");
  Serial.println(total);
  Serial.println(sinDuration);
  Serial.println(sinAmp);
  Serial.println(sinOffset);



  if (total < 5000) {
    ledState = INACTIVE;
  } else if (total > 5000 && total < 25000) {
    ledState = CLOSEPROXIMITY;
  } else if (total > 25000 && total < 50000) {
    ledState = TOUCH;
  } else if (total > 50000) {
    ledState = ANGRY;
  }
  delay(50);

  int brightness = sinewave(sinDuration, sinAmp, sinOffset); // our main variable for setting the brightness of the LED

}

void compose() {

  switch (ledState) {

    case INACTIVE:
      sinDuration = 1000;
      sinAmp = 15;
      sinOffset = 5;
      break;

    case CLOSEPROXIMITY:
      sinDuration = 750;
      sinAmp = 100;
      sinOffset = 5;
      break;

    case TOUCH:
      sinDuration = 500;
      sinAmp = 150;
      sinOffset = 5;
      break;

    case ANGRY:
      sinDuration = 50;
      sinAmp = 255;
      sinOffset = 1;
      break;
  }
}


int smooth_sensor(int sensor_value) {
  total = total - readings[readIndex];
  readings[readIndex] = sensor_value;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) {
    readIndex = 0;
  }

  average = total / numReadings;
  return average;
}


