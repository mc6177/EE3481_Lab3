// PART 2
// Ultrasonic sensor (time-based input) + photocell (voltage input)
// -> control RGB LED color and brightness

// Ultrasonic sensor pins
const int trigPin = 7;    // TRIG: (OUT) sends signal
const int echoPin = 6;    // ECHO: (IN) receives signal

// PWM pins: Control brightness
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

// photocell divider output
const int photocellPin = A0;

void setup() {
  Serial.begin(9600);
  
  // Ultrasonic pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // PWM pins
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  // Photocell
  int lightRaw = analogRead(photocellPin);
  int brightness = lightRaw / 4; // 0–255

  // Ultrasonic
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // from datasheet (https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf)
  digitalWrite(trigPin, LOW);

  // Measure echo pulse length (Time for sound to go out & back thru ultrasonic) 
  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * (0.0343/2); // speed of sound 343 m/s, distance = (duration * speed)/2 --> speed/2 = 0.343/2

  // Map distance to PWM 
  // Distance range: 5-80cm to 0-255 for LED
  int mappedValue = map(distance, 5, 80, 0, 255);
  mappedValue = constrain(mappedValue, 0, 255); // constrain: keep valid for PWM

  // (a) COLOR MAPPING
  // Green: closer distance
  // Red: farther distance
  
  int redValue   = mappedValue;          // increases w/ dist
  int greenValue = 255 - mappedValue;    // decreases w/ dist
  int blueValue  = 0;                    // not used here

  // (b) Apply brightness scaling
  redValue   = (redValue   * brightness) / 255;
  greenValue = (greenValue * brightness) / 255;

  // (c) LED - PWM OUTPUT 
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);

  // Print
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm   | Brightness: ");
  Serial.println(brightness);

  delay(1000);
}
