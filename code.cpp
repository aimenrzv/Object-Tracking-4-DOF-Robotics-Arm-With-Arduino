#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SoftwareSerial.h>
#include <NewPing.h>


Adafruit_PWMServoDriver PWM = Adafruit_PWMServoDriver();


const int RIGHT = A2;               // Right IR sensor connected to analog pin A2 of Arduino Uno
const int LEFT = A3;                // Left IR sensor connected to analog pin A3 of Arduino Uno
const int TRIGGER_PIN = A1;         // Trigger pin connected to analog pin A1 of Arduino Uno
const int ECHO_PIN = A0;            // Echo pin connected to analog pin A0 of Arduino Uno
const int MAX_DISTANCE = 200;       // Maximum ping distance


unsigned int distance = 0;          // Variable to store ultrasonic sensor distance
unsigned int Right_Value = 0;       // Variable to store Right IR sensor value
unsigned int Left_Value = 0;        // Variable to store Left IR sensor value


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);  // NewPing setup of pins and maximum distance


const int servo1 = 0;
const int servo2 = 1;
const int servo3 = 2;
const int servo4 = 3;


int Servo1Degree = 150;
int Servo2Degree = 150;
int Servo3Degree = 150;
int Servo4Degree = 325;


void setup() {
  Serial.begin(9600);
  pinMode(RIGHT, INPUT); // Set analog pin RIGHT as an input
  pinMode(LEFT, INPUT);  // Set analog pin LEFT as an input


  PWM.begin();
  PWM.setPWMFreq(60);
  PWM.setPWM(servo1, 0, Servo1Degree);
  PWM.setPWM(servo2, 0, Servo2Degree);
  PWM.setPWM(servo3, 0, Servo3Degree);
  PWM.setPWM(servo4, 0, Servo4Degree);
  delay(3000);
}


void loop() {
  delay(50);                                      // Wait 50ms between pings
  distance = sonar.ping_cm();                     // Send ping, get distance in cm and store it in 'distance' variable
  Serial.print("Distance: ");
  Serial.println(distance);                       // Print the distance in the serial monitor
  Right_Value = digitalRead(RIGHT);               // Read the value from Right IR sensor
  Left_Value = digitalRead(LEFT);                 // Read the value from Left IR sensor


  Serial.print("RIGHT: ");
  Serial.println(Right_Value);                    // Print the right IR sensor value in the serial monitor
  Serial.print("LEFT: ");
  Serial.println(Left_Value);                     // Print the left IR sensor value in the serial monitor


  if ((distance > 15) && (distance < 25)) {        // Check whether the ultrasonic sensor's value stays between 15 to 25.
    // Move Forward:
    Serial.println("Move Forward");
    PWM.setPWM(servo2, 0, (Servo2Degree += 3));


  } else if ((Right_Value == 0) && (Left_Value == 0)) {
    // Move Right
    Serial.println("Move Right");
    PWM.setPWM(servo1, 0, Servo1Degree += 3);


  } else if ((Right_Value == 1) && (Left_Value == 1)) {
    // Move Left
    Serial.println("Move Left");
    PWM.setPWM(servo1, 0, Servo1Degree -= 3);


  } else if ((distance > 5) && (distance < 15)) {
    // Move Backward
    Serial.println("Move Backward");
    PWM.setPWM(servo2, 0, Servo2Degree -= 3);


  } else if ((distance > 25) && (distance < 35)) {
    // Move Downward
    Serial.println("Move Downward");
    PWM.setPWM(servo3, 0, Servo3Degree -= 3);
  } else if ((distance > 35) && (distance < 45)) {
    // Move Upward
    Serial.println("Move Upward");
    PWM.setPWM(servo3, 0, Servo3Degree += 3);


  } else if ((distance > 1) && (distance <= 5)) {
    // Open Finger
    PWM.setPWM(servo4, 0, 325);
    delay(150);
    PWM.setPWM(servo4, 0, 400);
    delay(900);
    PWM.setPWM(servo4, 0, 325);


  } else if ((distance > 40) && (Right_Value == 1) && (Left_Value == 0)) {
    // Move Stop
    Serial.println("Move Stop");
    PWM.setPWM(servo1, 0, Servo1Degree);
    PWM.setPWM(servo2, 0, Servo2Degree);
    PWM.setPWM(servo3, 0, Servo3Degree);
    PWM.setPWM(servo4, 0, 325);
  }
}
