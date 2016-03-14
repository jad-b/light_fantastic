/****************************************
 * Example Sound Level Sketch for the 
 * Adafruit Microphone Amplifier
 ****************************************/
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
double bar;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
// Serial.fprintln("I have %d LEDs", strip.numPixels());


void setup() 
{
  Serial.begin(9600);

  strip.begin();
  strip.show();
  
  bar = 0.0;
}


void loop() {
  unsigned long startMillis= millis();  // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(0);
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax) {
        signalMax = sample;  // save just the max levels
      } 
      else if (sample < signalMin) {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  double voltage = 3.3;
  double volts = (peakToPeak * voltage) / 1024;  // convert to volts

  int n_lights = strip.numPixels();
  double n_lights_on = floor(n_lights / voltage * volts);

  zeroLights();

  for(uint16_t i=0; i <= n_lights_on; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 255));
  }


  if (bar < n_lights_on) {
    bar = n_lights_on + 1;
  } 
  else {
    bar = n_lights_on * 0.2 + bar * 0.8;
  }
  strip.setPixelColor(floor(bar), strip.Color(255, 0, 0));
  strip.show(); 

  Serial.println(n_lights_on);
}

void zeroLights() {
  for(uint16_t i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0,0,0));
  } 
}


