#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Initialize the driver with your specific static address 0x60
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// PCA9685 standard servo settings (adjust if your servos stretch/buzz)
#define SERVOMIN  150 // Minimum pulse length count (0 degrees)
#define SERVOMAX  600 // Maximum pulse length count (180 degrees)

// ==========================================
// HARDCODED STANDING ANGLES (No Math Needed)
// ==========================================
// Adjust these 12 numbers until your dog stands perfectly.

// Front Left Leg
const int FL_HIP   = 90;
const int FL_THIGH = 100; 
const int FL_KNEE  = 75; 

// Front Right Leg
const int FR_HIP   = 90;
const int FR_THIGH = 85; 
const int FR_KNEE  = 105;

// Back Left Leg
const int BL_HIP   = 70;
const int BL_THIGH = 115;
const int BL_KNEE  = 115;

// Back Right Leg
const int BR_HIP   = 100;
const int BR_THIGH = 65;
const int BR_KNEE  = 85;

void setup() {
  // Changed baud rate to 9600, which is standard for Arduino debugging
  Serial.begin(9600);
  Serial.println("Robotic Dog Standing Sequence Initializing...");

  // Standard Arduino hardware I2C initialization (No pin arguments needed)
  Wire.begin(); 
  
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60Hz

  delay(1000);
  
  // Command the dog to stand up immediately on boot
  standUp();
}

void loop() {
  // Keeping the dog standing statically.
}

// Function to convert degrees (0-180) to PCA9685 pulse width ticks
int angleToPulse(int angle) {
  return map(angle, 0, 180, SERVOMIN, SERVOMAX);
}

void standUp() {
  Serial.println("Moving servos to standing angles...");

  // ---- LEG 1: FRONT LEFT ----
  pwm.setPWM(0, 0, angleToPulse(FL_HIP));
  pwm.setPWM(1, 0, angleToPulse(FL_THIGH));
  pwm.setPWM(2, 0, angleToPulse(FL_KNEE));

  // ---- LEG 2: FRONT RIGHT ----
  pwm.setPWM(3, 0, angleToPulse(FR_HIP));
  pwm.setPWM(4, 0, angleToPulse(FR_THIGH));
  pwm.setPWM(5, 0, angleToPulse(FR_KNEE));

  // ---- LEG 3: BACK LEFT ----
  pwm.setPWM(12, 0, angleToPulse(BL_HIP));
  pwm.setPWM(13, 0, angleToPulse(BL_THIGH));
  pwm.setPWM(14, 0, angleToPulse(BL_KNEE));

  // ---- LEG 4: BACK RIGHT ----
  pwm.setPWM(9, 0, angleToPulse(BR_HIP));
  pwm.setPWM(10, 0, angleToPulse(BR_THIGH));
  pwm.setPWM(11, 0, angleToPulse(BR_KNEE));

  Serial.println("Dog is now standing.");
}
