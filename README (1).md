# Object Tracking 4-DOF Robotic Arm (Arduino)

A 4-DOF robotic arm that tracks and follows a moving object using an ultrasonic sensor and two IR sensors. When the object moves left, right, up, or down, the arm's servos respond in the corresponding direction.

## How It Works

- An **ultrasonic sensor** (HC-SR04) measures distance to the tracked object, driving forward/backward and up/down motion based on distance thresholds.
- Two **IR sensors** (LM393) detect whether the object has moved left or right.
- An **Arduino Uno** reads all sensor input and sends servo commands over I2C to a **PCA9685 PWM driver**, which drives the four **SG90 servo motors** (base, left, right, gripper).

## Components

| Qty | Part |
|---|---|
| 1 | Arduino Uno Rev3 |
| 1 | Ultrasonic Sensor Module — HC-SR04 |
| 2 | IR Sensor Module — LM393 |
| 1 | 4-DOF Acrylic Robotic Arm Kit |
| 1 | 16-Channel 12-bit PWM/Servo Driver (I2C, PCA9685) |
| 4 | Tower Pro SG90 Servo Motors |
| 1 | Soldering Iron Kit |

**Software:** Arduino IDE, plus the `Adafruit_PWMServoDriver` and `NewPing` libraries.

## Build Steps

1. **Assemble the arm kit** — follow the kit's mechanical assembly instructions.
2. **Wire the servos to the PWM driver:**
   - Gripper servo → PWM channel 0
   - Right-side servo → PWM channel 1
   - Left-side servo → PWM channel 2
   - Base servo → PWM channel 3
3. **Wire the PWM driver to the Arduino:**
   | PCA9685 | Arduino Uno |
   |---|---|
   | GND | GND |
   | SCL | A4 |
   | SDA | A5 |
   | VCC | VIN |
4. **Mount the sensors** — fit the ultrasonic sensor into its case, attach the case to the arm, and glue the IR sensors to the left and right sides of the case.
5. **Wire the sensors to the Arduino:**

   Ultrasonic sensor:
   | HC-SR04 | Arduino Uno |
   |---|---|
   | GND | GND |
   | ECHO | A3 |
   | TRIG | A2 |
   | VCC | VIN |

   Right IR sensor:
   | LM393 | Arduino Uno |
   |---|---|
   | GND | GND |
   | OUT | A1 |
   | VCC | VIN |

   Left IR sensor:
   | LM393 | Arduino Uno |
   |---|---|
   | GND | GND |
   | OUT | A0 |
   | VCC | VIN |

6. **Upload the sketch** (see `arm_tracking.ino` below) via USB.

## Code

```cpp
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SoftwareSerial.h>
#include <NewPing.h>

Adafruit_PWMServoDriver PWM = Adafruit_PWMServoDriver();

const int RIGHT = A2;               // Right IR sensor
const int LEFT = A3;                // Left IR sensor
const int TRIGGER_PIN = A1;         // Ultrasonic trigger
const int ECHO_PIN = A0;            // Ultrasonic echo
const int MAX_DISTANCE = 200;       // Max ping distance (cm)

unsigned int distance = 0;
unsigned int Right_Value = 0;
unsigned int Left_Value = 0;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

const int servo1 = 0;   // base
const int servo2 = 1;   // right
const int servo3 = 2;   // left
const int servo4 = 3;   // gripper

int Servo1Degree = 150;
int Servo2Degree = 150;
int Servo3Degree = 150;
int Servo4Degree = 325;

void setup() {
  Serial.begin(9600);
  pinMode(RIGHT, INPUT);
  pinMode(LEFT, INPUT);
  PWM.begin();
  PWM.setPWMFreq(60);
  PWM.setPWM(servo1, 0, Servo1Degree);
  PWM.setPWM(servo2, 0, Servo2Degree);
  PWM.setPWM(servo3, 0, Servo3Degree);
  PWM.setPWM(servo4, 0, Servo4Degree);
  delay(3000);
}

void loop() {
  delay(50);
  distance = sonar.ping_cm();
  Serial.print("Distance: ");
  Serial.println(distance);

  Right_Value = digitalRead(RIGHT);
  Left_Value = digitalRead(LEFT);
  Serial.print("RIGHT: ");
  Serial.println(Right_Value);
  Serial.print("LEFT: ");
  Serial.println(Left_Value);

  if ((distance > 15) && (distance < 25)) {
    Serial.println("Move Forward");
    PWM.setPWM(servo2, 0, (Servo2Degree += 3));
  } else if ((Right_Value == 0) && (Left_Value == 0)) {
    Serial.println("Move Right");
    PWM.setPWM(servo1, 0, Servo1Degree += 3);
  } else if ((Right_Value == 1) && (Left_Value == 1)) {
    Serial.println("Move Left");
    PWM.setPWM(servo1, 0, Servo1Degree -= 3);
  } else if ((distance > 5) && (distance < 15)) {
    Serial.println("Move Backward");
    PWM.setPWM(servo2, 0, Servo2Degree -= 3);
  } else if ((distance > 25) && (distance < 35)) {
    Serial.println("Move Downward");
    PWM.setPWM(servo3, 0, Servo3Degree -= 3);
  } else if ((distance > 35) && (distance < 45)) {
    Serial.println("Move Upward");
    PWM.setPWM(servo3, 0, Servo3Degree += 3);
  } else if ((distance > 1) && (distance <= 5)) {
    // Open gripper
    PWM.setPWM(servo4, 0, 325);
    delay(150);
    PWM.setPWM(servo4, 0, 400);
    delay(900);
    PWM.setPWM(servo4, 0, 325);
  } else if ((distance > 40) && (Right_Value == 1) && (Left_Value == 0)) {
    Serial.println("Move Stop");
    PWM.setPWM(servo1, 0, Servo1Degree);
    PWM.setPWM(servo2, 0, Servo2Degree);
    PWM.setPWM(servo3, 0, Servo3Degree);
    PWM.setPWM(servo4, 0, 325);
  }
}
```

## License

GPL-3.0-or-later
