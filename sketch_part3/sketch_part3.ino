#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Display setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 disp(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Rotary encoder pins
const int A = 2;
const int B = 3;

// encoder state
int lastA;
int encoderValue = 128;

unsigned long lastEncoderStep = 0;
const int encoderDebounce = 3;

// Ball state
int x = 64;
int y = 32;

int vx = 0;
int vy = 0;

// fixed direction vector!!
int dx = 3;
int dy = 2;

const int r = 4;

// timing for debounce / animation
unsigned long lastStep = 0;
const int stepDelay = 15;


// SETUP
void setup() {

  Serial.begin(9600);

  pinMode(A, INPUT_PULLUP);
  pinMode(B, INPUT_PULLUP);

  lastA = digitalRead(A);

  if(!disp.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
    while(true);
  }

  disp.clearDisplay();
  disp.display();
}


// ROTARY ENCODER UPDATE
void updateEncoder() {

  int currentA = digitalRead(A);

  if (currentA != lastA) {

    // debounce check
    if (millis() - lastEncoderStep > encoderDebounce) {

      if (digitalRead(B) != currentA) {
        encoderValue++;
      } else {
        encoderValue--;
      }

      encoderValue = constrain(encoderValue, 0, 255);

      Serial.println(encoderValue);

      lastEncoderStep = millis();
    }
  }

  lastA = currentA;
}


// UPDATE BALL PHYSICS
void updateBall() {

  int speed = encoderValue - 128;

  int scaled = speed / 16;

  if (scaled == 0 && speed != 0) {
    scaled = (speed > 0) ? 1 : -1;
  }

  vx = scaled * dx;
  vy = scaled * dy;

  x += vx;
  y += vy;

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
}

// DRAW BALL
void drawBall() {
  disp.clearDisplay();
  disp.fillCircle(x, y, r, WHITE);
  disp.display();
}


// MAIN LOOP
void loop() {

  updateEncoder();

  if (millis() - lastStep > stepDelay) {

    updateBall();
    drawBall();

    lastStep = millis();
  }

}
