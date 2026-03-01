
// PART 1:
// PHOTOCELL -> brightness value -> LED brightness
/*The brightness of the LED is directly proportional to the photocell reading. 
As the photocell reading increases, the PWM duty cycle increases, 
resulting in a brighter LED. Conversely, a lower photocell reading produces a dimmer LED.
*/

const int photocellPin = A0;  // PHOTOCELL: connected to A0
const int ledPin = 11;        // LED: constant; connected to pin 11


// Setup
void setup() {
 Serial.begin(9600);
 pinMode(ledPin, OUTPUT); // pin 11 -> send signals
}


// Loop
void loop() {
 // (1) Sensor: read light voltage from photocell (A0) -> ADC Out
 // Output range: (2^10) => 0 - 1023

 // (2) Convert ADC result -> Brightness
 // Out range: (0 - 1023) -> (0 - 255) (for analogwrite)
  int out = analogRead(photocellPin);  //(1)
  int brightness = out / 4;  //(2)

 // analogWrite -> PWM signal (rapidly switch output pin b/w HIGH & LOW)
 // duty cycle of signal -> value b/w 0 - 255 -> controls avg voltage -> determines brightness
  analogWrite(ledPin, brightness); // set LED pin tp brightness

// printing out raw (photocell values) & corresponding LED brightness
  Serial.print("Raw: ");
  Serial.print(out);

  Serial.print(" | Brightness: ");
  Serial.println(brightness);

  delay(200);
}

