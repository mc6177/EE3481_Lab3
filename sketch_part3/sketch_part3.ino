// PART 3
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Display setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
// Create display obj using I2C comm
Adafruit_SSD1306 disp(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Rotary encoder pins + inits
const int A = 2;
const int B = 3;
int lastA;                // stores prev state of encoder pin A to detect transitiosn
int encoderValue = 128;   // init to 0 velocity
// encoder debouncing inits
unsigned long lastEncoderStep = 0;
const int encoderDebounce = 3;

// Ball state
// Ball position
int x = 64;
int y = 32;
// Velocity Compoennts
int vx = 0;
int vy = 0;
// Fixed direction vector
int dx = 3;
int dy = 2;

const int r = 4;

// Timing Control
unsigned long lastStep = 0;
const int stepDelay = 15;


// SETUP
void setup() {
  Serial.begin(9600);

  pinMode(A, INPUT_PULLUP);
  pinMode(B, INPUT_PULLUP);

  lastA = digitalRead(A); // record init state of encoder pin A

  // Init OLED display over I2C
  if(!disp.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
    while(true);
  }

  disp.clearDisplay();
  disp.display();
}


// ROTARY ENCODER UPDATE
// Read the rotary encoder and updates encoderValue
// Debouncing is implemented using a min time between valid transitions
void updateEncoder() {
  int currentA = digitalRead(A);

  // Change in encoder signal
  if (currentA != lastA) {
    // Simple debounce check
    if (millis() - lastEncoderStep > encoderDebounce) {
      // Determine rotation direction using quadrature relationship
      if (digitalRead(B) != currentA) {
        encoderValue++; // clockwise
      } else {
        encoderValue--; // counter-clockwise
      }

      encoderValue = constrain(encoderValue, 0, 255); // ensure encoder value is within bounds
      Serial.println(encoderValue);
      lastEncoderStep = millis(); // record time of last valid step
    }
  }
  // Save current state for next comparison
  lastA = currentA;
}


// UPDATE BALL PHYSICS
// encoderValue > 128 : Pos velocity along dir vector
// encoderValue = 128 : Velocity = 0
// encoderValue > 128 : Neg velocity along dir vector
void updateBall() {
  // Convert encoder value into signed velocity
  int speed = encoderValue - 128;
  // Scale velocity (to not appear to fast)
  int scaled = speed / 16;
  if (scaled == 0 && speed != 0) {
    scaled = (speed > 0) ? 1 : -1;
  }

  // Compute actual velocity components using the direction vector
  vx = scaled * dx;
  vy = scaled * dy;

  // Update ball positions
  x += vx;
  y += vy;

  // COLLISION DETECTION & BOUNCING
  // TOP WALL
  if (y <= r) {
    y = r;
    dy = -dy;
  }
  // BOTTOM WALL
  if (y >= SCREEN_HEIGHT - r) {
    y = SCREEN_HEIGHT - r;
    dy = -dy;
  }
  // LEFT WALL
  if (x <= r) {
    x = r;
    dx = -dx;
  }
  // RIGHT WALL
  if (x >= SCREEN_WIDTH - r) {
    x = SCREEN_WIDTH - r;
    dx = -dx;
  }
}

// DRAW BALL
void drawBall() {
  disp.clearDisplay();
  disp.fillCircle(x, y, r, WHITE);
  disp.display();
}


// MAIN LOOP
void loop() {
  // Read rotary encoder input
  updateEncoder();
  // Update animation (with a controlld frame rate)
  if (millis() - lastStep > stepDelay) {
    updateBall();
    drawBall();
    
    lastStep = millis();
  }

}
