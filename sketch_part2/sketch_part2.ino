// PART 2

const int photocellPin = A0;
const int trigPin = 7;
const int echoPin = 6;

const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  // photocell
  int lightRaw = analogRead(photocellPin);
  int brightness = lightRaw / 4; // 0–255

  // ultrasonic
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // from datasheet (https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf)
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * (0.0343/2); // speed of sound 343 m/s, distance = (duration * speed)/2 --> speed/2 = 0.343/2

  int mappedValue = map(distance, 5, 80, 0, 255);
  mappedValue = constrain(mappedValue, 0, 255); // keep valid for PWM

  // COLOR MAPPING
  int redValue   = mappedValue;
  int greenValue = 255 - mappedValue;
  int blueValue  = 0;

  // apply brightness scaling
  redValue   = (redValue   * brightness) / 255;
  greenValue = (greenValue * brightness) / 255;

  // LED
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);

  // print
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm   | Brightness: ");
  Serial.println(brightness);

  delay(1000);
}
