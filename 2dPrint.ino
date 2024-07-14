#include <Servo.h>
#include <Stepper.h>

// Servo motor configuration
Servo myServo;
int servoPin = 13;  // Servo motor connected to digital pin 9
int joyButtonPin = 0;  // Joystick button connected to digital pin 2
bool lastButtonState = LOW;  // Previous state of the joystick button
bool currentButtonState = LOW;  // Current state of the joystick button
bool servoPosition = false;  // False for 0 degrees, True for 90 degrees

// Stepper motor configuration
const int stepsPerRevolution = 2048;  // Change this to fit the number of steps per revolution
const int rolePerMinute = 15;
const int X_pin = 1; // Analog pin connected to X output
const int Y_pin = 0; // Analog pin connected to Y output

Stepper myStepperX(stepsPerRevolution, 2, 4, 3, 5);
Stepper myStepperY(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  // Setup for servo motor
  myServo.attach(servoPin);  // Attach the servo to the pin
  pinMode(joyButtonPin, INPUT);  // Set the joystick button pin as input
  myServo.write(0);  // Initial position of the servo

  // Setup for stepper motors
  myStepperX.setSpeed(rolePerMinute);
  myStepperY.setSpeed(rolePerMinute);
}

void loop() {
  // Servo motor control
  currentButtonState = digitalRead(joyButtonPin);
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    // Toggle the servo position
    if (servoPosition) {
      myServo.write(0);  // Move to 0 degrees
    } else {
      myServo.write(90);  // Move to 90 degrees
    }
    servoPosition = !servoPosition;  // Toggle the position flag
    delay(50);  // Debounce delay
  }
  lastButtonState = currentButtonState;  // Update the last button state

  // Stepper motor control
  int a = analogRead(X_pin);
  int b = analogRead(Y_pin);

  // Determine the direction and speed for X axis (a)
  int stepX = 0;
  if (a < 300) {
    stepX = -30;
  } else if (a > 724) {
    stepX = 30;
  }

  // Determine the direction and speed for Y axis (b)
  int stepY = 0;
  if (b < 300) {
    stepY = -30;
  } else if (b > 724) {
    stepY = 30;
  }

  // Perform the steps for X and Y axis
  if (stepX != 0 && stepY != 0) {
    myStepperX.setSpeed(rolePerMinute);
    myStepperX.step(stepX);
    myStepperY.setSpeed(rolePerMinute);
    myStepperY.step(stepY);
  } else if (stepX != 0) {
    myStepperX.setSpeed(rolePerMinute);
    myStepperX.step(stepX);
  } else if (stepY != 0) {
    myStepperY.setSpeed(rolePerMinute);
    myStepperY.step(stepY);
  } else {
    for (int i = 2; i < 6; i++) {
      digitalWrite(i, LOW);
    }
    for (int i = 8; i < 12; i++) {
      digitalWrite(i, LOW);
    }
  }
}
