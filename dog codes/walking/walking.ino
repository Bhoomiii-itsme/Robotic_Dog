#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Initialize the driver with your specific static address 0x60
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x60);

#define SERVOMIN  150 // Minimum pulse length count (0 degrees)
#define SERVOMAX  600 // Maximum pulse length count (180 degrees)

// ==========================================
// YOUR NEW FINALISED STANDING ANGLES
// ==========================================
const int FL_HIP   = 90;   const int FL_THIGH = 100;   const int FL_KNEE  = 75;
const int FR_HIP   = 90;   const int FR_THIGH = 85;  const int FR_KNEE  = 105;
const int BL_HIP   = 70;   const int BL_THIGH = 115;  const int BL_KNEE  = 115;
const int BR_HIP   = 100;  const int BR_THIGH = 65;   const int BR_KNEE  = 85;

// Function prototypes so loop() can see them seamlessly
int angleToPulse(int angle);
void standUp();

// Global flag to control the 15-second delay initialization
bool startupDelayComplete = false; 

void setup() {
  // Changed baud rate to 9600 for Arduino standard serial monitoring
  Serial.begin(9600);
  Serial.println("Robotic Dog Walking Sequence Initializing...");

  // Standard Arduino hardware I2C initialization (Pins fixed by hardware)
  Wire.begin(); 
  
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60Hz

  delay(1000);
  
  // Bring the dog up to its default standing position immediately
  standUp();
  Serial.println("\n=========================================");
  Serial.println("Dog is standing. Commencing 15-second stabilization wait...");
  Serial.println("=========================================");
}

void loop() {
  // Execute the 15-second countdown just once at startup
  if (!startupDelayComplete) {
    for (int remaining = 15; remaining > 0; remaining--) { // Fixed code logic to match your 15-sec comment
      Serial.print("Time remaining until movement: ");
      Serial.print(remaining);
      Serial.println(" seconds...");
      delay(1000); // 1-second ticks
    }
    Serial.println("\n>>> 15 Seconds Up! Beginning Stride Sequence! <<<\n");
    startupDelayComplete = true; 
  }

  Serial.println("--- Starting Full 4-Leg Gait Cycle ---");
  
  int totalSteps = 6; // Controls smoothness of individual joint sweeps
  int stepDelay = 3;  // Fast execution delay
  int interLegPause = 150; // Pause between different legs for balance stability

  // ==========================================================
  // 1. FRONT RIGHT (FR) LEG PHASE
  // ==========================================================
  Serial.println(">> 1. Moving FRONT RIGHT...");
  for (int i = 0; i <= totalSteps; i++) {
    int offset = map(i, 0, totalSteps, 0, 15);
    pwm.setPWM(4, 0, angleToPulse(FR_THIGH + offset)); 
    delay(stepDelay); 
  }
  for (int i = 0; i <= totalSteps; i++) {
    int offset = map(i, 0, totalSteps, 0, 15);
    pwm.setPWM(5, 0, angleToPulse(FR_KNEE + offset));   
    delay(stepDelay);
  }
  delay(150);
  for (int i = 0; i <= totalSteps; i++) {
    int currentKnee = map(i, 0, totalSteps, FR_KNEE + 15, FR_KNEE);
    pwm.setPWM(5, 0, angleToPulse(currentKnee)); 
    delay(stepDelay);
  }
  for (int i = 0; i <= totalSteps; i++) {
    int currentThigh = map(i, 0, totalSteps, FR_THIGH + 15, FR_THIGH);
    pwm.setPWM(4, 0, angleToPulse(currentThigh)); 
    delay(stepDelay);
  }
  delay(interLegPause);

  // ==========================================================
  // 2. BACK LEFT (BL) LEG PHASE
  // ==========================================================
  Serial.println(">> 2. Moving BACK LEFT...");
  for (int i = 0; i <= totalSteps; i++) {
    int offset = map(i, 0, totalSteps, 0, 15); 
    pwm.setPWM(13, 0, angleToPulse(BL_THIGH - offset)); 
    delay(stepDelay); 
  }
  for (int i = 0; i <= totalSteps; i++) {
    int offset = map(i, 0, totalSteps, 0, 15);
    pwm.setPWM(14, 0, angleToPulse(BL_KNEE - offset));  
    delay(stepDelay);
  }
  delay(150);
  for (int i = 0; i <= totalSteps; i++) {
    int currentKnee = map(i, 0, totalSteps, BL_KNEE - 15, BL_KNEE); 
    pwm.setPWM(14, 0, angleToPulse(currentKnee)); 
    delay(stepDelay);
  }
  for (int i = 0; i <= totalSteps; i++) {
    int currentBLThigh = map(i, 0, totalSteps, BL_THIGH - 15, BL_THIGH); 
    pwm.setPWM(13, 0, angleToPulse(currentBLThigh)); 
    delay(stepDelay);
  }
  delay(interLegPause);

  // ==========================================================
  // 3. FRONT LEFT (FL) LEG PHASE
  // ==========================================================
  Serial.println(">> 3. Moving FRONT LEFT...");
  for (int i = 0; i <= totalSteps; i++) {
    int offset = map(i, 0, totalSteps, 0, 15);
    pwm.setPWM(1, 0, angleToPulse(FL_THIGH - offset)); 
    delay(stepDelay); 
  }
  for (int i = 0; i <= totalSteps; i++) {
    int offset = map(i, 0, totalSteps, 0, 15);
    pwm.setPWM(2, 0, angleToPulse(FL_KNEE - offset));   
    delay(stepDelay);
  }
  delay(150);
  for (int i = 0; i <= totalSteps; i++) {
    int currentKnee = map(i, 0, totalSteps, FL_KNEE - 15, FL_KNEE);
    pwm.setPWM(2, 0, angleToPulse(currentKnee)); 
    delay(stepDelay);
  }
  for (int i = 0; i <= totalSteps; i++) {
    int currentThigh = map(i, 0, totalSteps, FL_THIGH - 15, FL_THIGH);
    pwm.setPWM(1, 0, angleToPulse(currentThigh)); 
    delay(stepDelay);
  }
  delay(interLegPause);

  // ==========================================================
  // 4. BACK RIGHT (BR) LEG PHASE
  // ==========================================================
  Serial.println(">> 4. Moving BACK RIGHT...");
  for (int i = 0; i <= totalSteps; i++) {
    int offset = map(i, 0, totalSteps, 0, 15); 
    pwm.setPWM(10, 0, angleToPulse(BR_THIGH + offset)); 
    delay(stepDelay); 
  }
  for (int i = 0; i <= totalSteps; i++) {
    int offset = map(i, 0, totalSteps, 0, 15);
    pwm.setPWM(11, 0, angleToPulse(BR_KNEE + offset));  
    delay(stepDelay);
  }
  delay(150);
  for (int i = 0; i <= totalSteps; i++) {
    int currentKnee = map(i, 0, totalSteps, BR_KNEE + 15, BR_KNEE); 
    pwm.setPWM(11, 0, angleToPulse(currentKnee)); 
    delay(stepDelay);
  }
  for (int i = 0; i <= totalSteps; i++) {
    int currentThigh = map(i, 0, totalSteps, BR_THIGH + 15, BR_THIGH); 
    pwm.setPWM(10, 0, angleToPulse(currentThigh)); 
    delay(stepDelay);
  }

  Serial.println("--- Full Gait Cycle Complete. Restarting loop ---");
  delay(1000); 
}

// ==========================================
// HELPER FUNCTIONS
// ==========================================

int angleToPulse(int angle) {
  return map(angle, 0, 180, SERVOMIN, SERVOMAX);
}

void standUp() {
  Serial.println("Setting all legs to baseline standing angles...");

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
}