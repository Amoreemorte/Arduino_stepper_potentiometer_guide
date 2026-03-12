// ============================================
// 28BYJ-48 STEPPER MOTOR CONTROL
// WITH POTENTIOMETER
// ============================================

// --- DEBUG SETTINGS ---
// Uncomment the following line to enable debugging
//#define DEBUG

// --- HARDWARE CONNECTION ---
// Potentiometer: middle pin -> Arduino analog pin A0
// Outer pins: GND and 5V on Arduino
#define SENSOR_PIN A0 

// ULN2003 Driver:
// Driver Input|Arduino Digital Pin|
// ------------|-------------------|
// IN1         | PIN_8             |
// IN2         | PIN_9             |
// IN3         | PIN_10            |
// IN4         | PIN_11            |
#define PIN_1 8
#define PIN_2 9
#define PIN_3 10
#define PIN_4 11

// --- MOTION SETTINGS ---
// Delay time between microsteps (milliseconds)
// Recommendations:
// 1-2 ms - fast, but may skip steps
// 3-5 ms - optimal for 28BYJ-48
#define DELAY_TIME 2

// Potentiometer threshold value (changes when rotating)
// When crossed, the motor starts rotating in the opposite direction
// Recommended to use 1023/2 +- 100
#define THRESHOLD_POTENTIOMETER_VALUE 600

// --- HALF-STEP SEQUENCE TABLE ---
// Sequence for 28BYJ-48 (half-step mode)
// 8 states = 1 full step
// Full rotation = 4096 states
int seq[8][4] = {
  {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};

// ============================================
// SETUP - runs once
// ============================================
void setup() {
  Serial.begin(9600); // Start communication with computer
  #ifdef DEBUG
      Serial.println("Connected to serial monitor"); 
  #endif
  // Initialize pins  
  pinMode(PIN_1, OUTPUT);
  pinMode(PIN_2, OUTPUT);
  pinMode(PIN_3, OUTPUT);
  pinMode(PIN_4, OUTPUT);
}

// Current voltage value on the analog pin connected to the potentiometer
int sensorValue = 0;

// ============================================
// LOOP - runs indefinitely
// ============================================
void loop() {
  sensorValue = analogRead(SENSOR_PIN); // Read value (0-1023)

  // Output to serial monitor
  // View via keyboard shortcut Ctrl+Shift+M 
  #ifdef DEBUG
      Serial.println(sensorValue); 
  #endif

  // Check if the analog pin value exceeds the threshold
  if (sensorValue > THRESHOLD_POTENTIOMETER_VALUE){
    for (int step = 0; step < 8; step++) {
      // Clockwise motor rotation      
      digitalWrite(PIN_1, seq[step][0]);
      digitalWrite(PIN_2, seq[step][1]);
      digitalWrite(PIN_3, seq[step][2]);
      digitalWrite(PIN_4, seq[step][3]);
      delay(DELAY_TIME);
    }
  } else {
    for (int step = 7; step >= 0; step--) {
      // Counter-clockwise motor rotation
      digitalWrite(PIN_1, seq[step][0]);
      digitalWrite(PIN_2, seq[step][1]);
      digitalWrite(PIN_3, seq[step][2]);
      digitalWrite(PIN_4, seq[step][3]);
      delay(DELAY_TIME); 
    }
  }
}