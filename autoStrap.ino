/* Example sketch to control a 28BYJ-48 stepper motor with ULN2003 driver board and Arduino UNO. More info: https://www.makerguides.com */

// Include the Arduino Stepper.h library:
#include <Stepper.h>
/*
  Resistance Measurement - www.circuits4you.com
  Reads an analog input on pin 0, converts it to resistance, and prints the result to the serial monitor.
*/

// Define number of steps per rotation:
const int stepsPerRevolution = 4096;
const int buttonPin = 2;
boolean isTight = false;
int buttonState = 0;
boolean tightening = false;
boolean loosening = false;
// Wiring:
// Pin 8 to IN1 on the ULN2003 driver
// Pin 9 to IN2 on the ULN2003 driver
// Pin 10 to IN3 on the ULN2003 driver
// Pin 11 to IN4 on the ULN2003 driver

// Create stepper object called 'myStepper', note the pin order:
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  pinMode(buttonPin, INPUT);
  // Set the speed to 5 rpm:
  myStepper.setSpeed(5);
}


// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  buttonState = digitalRead(buttonPin);
  int sensorValue = analogRead(A0);

  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1024.0);

  float I = voltage / 1000;
  float VRx = 5 - voltage;
  float Rx = VRx / I;
  Rx = (5 - voltage) / I;

  // print out the value you read:
  Serial.print("Resistance:");
  Serial.print(Rx);
  Serial.println(" Ohms");
  // delay(500);

  //to tighten the strap
  if (!isTight && buttonState == HIGH) {
    Serial.print("   Tightening!    ");
    delay(1000);
    tightening = true;
  }
  else if (tightening && Rx > 500) {
    myStepper.step(100);
  }
  else if (Rx < 500 && !isTight) {
    Serial.println("\n Done!");
    myStepper.step(100);
    tightening = false;
    isTight = true;
  }
  //loosening
  if (isTight && buttonState == HIGH && !tightening) {
    loosening = true;
  }
  if (loosening) {
    Serial.println("loosening");
    myStepper.step(-stepsPerRevolution / 2);
    delay(500);
    isTight = false;
    loosening = false;
    Serial.println("Loose!");
  }


}
