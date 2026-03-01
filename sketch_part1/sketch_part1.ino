// PART 1
// PHOTOCELL -> brightness value -> LED brightness
/*The brightness of the LED is directly proportional to the photocell reading. 
As the photocell reading increases, the PWM duty cycle increases, 
resulting in a brighter LED. Conversely, a lower photocell reading produces a dimmer LED.
*/

// (0) init
const int photocellPin = A0;  // PHOTOCELL: connected to A0
const int bluePin = 11;       // LED: constant; connected to pin 11

// Setup
void setup() {
 Serial.begin(9600);
 pinMode(bluePin, OUTPUT); // pin 11 -> send signals
}

// Loop
void loop() {
  // (1) Sensor: read light voltage from photocell (A0) -> ADC Out
  // Output range: (2^10) => 0 - 1023

  // (2) Convert ADC result -> Brightness
  // Out range: (0 - 1023) -> (0 - 255) (for analogwrite)
  int out = analogRead(photocellPin);  // (1)
  int brightness = out / 4;  // (2)

  // (3) analogWrite -> PWM signal (rapidly switch output pin b/w HIGH & LOW)
  // duty cycle of signal -> value b/w 0 - 255 -> controls avg voltage -> determines brightness
  analogWrite(bluePin, brightness); // set LED pin tp brightness

  // (4) printing out raw (photocell values) & corresponding LED brightness
  Serial.print("Brightness: ");
  Serial.println(brightness);

  delay(1000);
}


/*
"...how you can implement analogRead() using I/O mapped registers on the AVR (look at the appropriate section of the datasheet for the AVR)"

Registers involved:
- ADMUX  : channel selection & reference
- ADCSRA : enable, start conversion, status
- ADCL/ADCH : result registers

We can implement analogRead() using AVR's memory-mapped ADC registers like so:
1. Use the ADMUX register to select the analog input channel and use the AVCC asreference voltage
2. ADC is enabled by enabling ADEN bit and the prescaler bits (ADPS2:0) to divide the system clock by 128 (to get a reasonable ADC clock frequency)
3. Start conversion by setting the ADSC bit and poll the ADSC bit until it clears
4. The 10'b result can be read from the ADCL and ADCH registers and combined to a 16'b integer [0, 1023]

*/
