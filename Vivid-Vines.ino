#include <FastLED.h>


#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN 2
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS 50
#define BRIGHTNESS 1000
int flexPin1 = A1;
int flexPin2 = A2;
int flexData1 = 0;
int flexData2 = 0;
int touchPin[] = { 3, 4, 5, 6, 7 };
int touchVal[] = { 0, 0, 0, 0, 0 };
CRGB leds[NUM_LEDS];

const int sampleWindow = 50;  // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
#define MIC_PIN 0


// the setup routine runs once when you press reset:

void setup() {
  Serial.begin(9600);
  for (int pin = 3; pin < 8; pin++) {
    pinMode(pin, INPUT);
  }
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

// the loop routine runs over and over again forever:
void loop() {

  FastLED.setBrightness(255);
  flexData1 = analogRead(flexPin1);
  flexData2 = analogRead(flexPin2);
  double volts = getVoltage();
  getTouchVal();
  long starttime = millis();
  long endtime = starttime;

  //The following code is for the touch sensors
  if (touchVal[0] == 1 && touchVal[1] == 1 && touchVal[2] == 1 && touchVal[3] == 1 && touchVal[4] == 1){
    fadeOn(	255, 165, 0, 10);
  }
  if (touchVal[1] == 1 && touchVal[0] == 1) {
    explosion();
  } else if (getVoltage() > 4) {
    Serial.println("light level 4.9");
    microphone3();
    while (getVoltage() < 4) {
      leds[25] = CRGB::White;
      leds[24] = CRGB::White;
      FastLED.show();
      if (getVoltage() > 4) { break; }
      delay(5);
      leds[25] = CRGB::Black;
      leds[24] = CRGB::Black;
      FastLED.show();
    }
    idle();
    // } else if (getVoltage() > 2) {
    //   Serial.println("light level 3 - 4.9");
    //   microphone3();

  } else if (touchVal[0] == 1) {
    while ((touchVal[0] = digitalRead(touchPin[0])) == 1) {
      Serial.println("1 = 1");
      strobe();
    }
  } else if (touchVal[1] == 1) {
    while (digitalRead(touchPin[1])) {
      while (digitalRead(touchPin[1]) && digitalRead(touchPin[2])) {
        explosion();
      }
      Serial.println("3 = 1");
      cylon();
    }

  } else if (touchVal[2] == 1) {
    while (digitalRead(touchPin[2])) {
      pride();
      FastLED.show();
    }

  } else if (touchVal[3] == 1) {
    Serial.println("4 = 1");
    comet();

    rainbowColorShow();
    rainbowColorShowReverse();
  } else if (touchVal[4] == 1) {
    Serial.println("5 = 1");
    // //colorPulse();
    // //flashEvenOdd();
    // rainbow();
    fadeOn(220 , 96, 200, 6);
  } else if (flexData1 > 10) {

    while (analogRead(flexPin1) > 2) {
      flexData2 = analogRead(flexPin1);
      flashEvenOdd(flexData2 * 5);
    }
  } else if (flexData2 > 10) {

    while (analogRead(flexPin2) > 2) {
      flexData2 = analogRead(flexPin2);
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Yellow;
        //FastLED.show();
      }
      int newFlex = analogRead(flexPin2);
      if (flexData2 > newFlex) {
        for (int i = flexData2 * 5; i > newFlex * 5; i--) {
          FastLED.setBrightness(i);
          FastLED.show();
        }
      } else {
        for (int i = flexData2 * 5; i < newFlex * 5; i++) {
          FastLED.setBrightness(i);
          FastLED.show();
        }
      }

      Serial.println(analogRead(flexPin2));
    }
  } else {
    //fadeOn(60,179,113,0.001);
    idle();
  }
}

void idle() {
  for (int i = 0; i < NUM_LEDS; i++) {
    switch (i) {
      case 6:
        leds[6] = CRGB::Cyan;
        break;
      case 17:
        leds[17] = CRGB::Cyan;
        break;
      case 27:
        leds[27] = CRGB::Cyan;
        break;
      case 30:
        leds[30] = CRGB::Cyan;
        break;
      case 36:
        leds[36] = CRGB::Cyan;
        break;
      default:
        leds[i] = CRGB::Purple;
    }
  }
  FastLED.show();
}
void getTouchVal() {
  for (int i = 0; i < 5; i++) {
    touchVal[i] = digitalRead(touchPin[i]);
  }
}

void turnOff() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
    FastLED.show();
  }
  FastLED.setBrightness(255);
}
void microphone3() {
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = CRGB::Violet;
    leds[dot - 1] = CRGB::DarkViolet;
    leds[dot - 2] = CRGB::Indigo;
    leds[dot - 3] = CRGB::Black;
    FastLED.show();
    // clear this led for the next time around the loop
    leds[dot] = CRGB::Black;
    delay(15);
  }
  turnOff();
}

void microphone4() {
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = CRGB::Red;
    FastLED.show();
    // clear this led for the next time around the loop
    leds[dot] = CRGB::Black;
    delay(30);
  }
}
// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.
void pride() {
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88(87, 220, 250);
  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;  //gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);
    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS - 1) - pixelnumber;
    nblend(leds[pixelnumber], newcolor, 64);
  }
}

void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); }
}

void cyclone() {
  static uint8_t hue = 0;
  Serial.print("x");
  // First slide the led in one direction
  for (int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red
    leds[i] = CRGB::OrangeRed;
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }
  Serial.print("x");

  // Now go in the other direction.
  for (int i = (NUM_LEDS)-1; i >= 0; i--) {
    // Set the i'th led to red
    leds[i] = CRGB::OrangeRed;
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }
}
void rainbowColorShow() {
  for (int i = 0; i < NUM_LEDS - 9; i++) {
    //from leds i to i+8 make it light up like a rainbow
    leds[i] = CRGB::Black;
    leds[i + 1] = CRGB::Red;
    leds[i + 2] = CRGB::Orange;
    leds[i + 3] = CRGB::Yellow;
    leds[i + 4] = CRGB::Green;
    leds[i + 5] = CRGB::Blue;
    leds[i + 6] = CRGB::Indigo;
    leds[i + 7] = CRGB::Violet;
    leds[i + 8] = CRGB::HotPink;
    leds[i + 9] = CRGB::Pink;
    FastLED.show();
    delay(30);
  }
}
void rainbowColorShowReverse() {

  for (int i = NUM_LEDS - 1; i > 9; i--) {
    //from leds i to i+8 make it light up like a rainbow
    leds[i] = CRGB::Black;
    leds[i - 1] = CRGB::Red;
    leds[i - 2] = CRGB::Orange;
    leds[i - 3] = CRGB::Yellow;
    leds[i - 4] = CRGB::Green;
    leds[i - 5] = CRGB::Blue;
    leds[i - 6] = CRGB::Indigo;
    leds[i - 7] = CRGB::Violet;
    leds[i - 8] = CRGB::HotPink;
    leds[i - 9] = CRGB::Pink;
    FastLED.show();
    delay(30);
  }
}

void flashEvenOdd(int d) {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i % 2 == 0) {
      leds[i] = CRGB(255, 255, 0);
    } else {
      leds[i] = CRGB::Black;
    }
  }
  FastLED.show();
  delay(d);
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i % 2 == 0) {
      leds[i] = CRGB::Black;
    } else {
      leds[i] = CRGB::HotPink;
    }
  }
  FastLED.show();
  delay(d);
}

void rainbow() {
  // FastLED's built-in rainbow generator
  fill_rainbow(leds, NUM_LEDS, 13, 7);
}

void fadeOn(int r, int g, int b, double d) {
  for (int i = 0; i < 255 / 2; i++) {
    for (int j = 0; j < NUM_LEDS; j++) {
      leds[j] = CRGB(r, g, b);
      FastLED.setBrightness(i * 2);
    }
    FastLED.show();
    delay(d);
  }
  for (int i = 255 / 2; i > 0; i--) {
    for (int j = 0; j < NUM_LEDS; j++) {
      leds[j] = CRGB(r, g, b);
    }
    FastLED.setBrightness(i * 2);
    FastLED.show();
    delay(d);
  }
}

void comet() {
  static uint8_t hue = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue, 255, 255);
    FastLED.show();
    fadeall();
    delay(20);
  }
  hue++;
}
//
void twinkle() {
  int randomLed = random(NUM_LEDS);
  leds[randomLed] = CHSV(random(0, 255), 255, 255);
  FastLED.show();
  delay(50);
  leds[randomLed] = CRGB::Black;
}

void strobe() {
  for (int j = 0; j < 10; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::White;
    }
    FastLED.show();
    if (getVoltage() > 2.9) { cyclone(); }
    delay(30);
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }
    FastLED.show();
    if (getVoltage() > 2.9) {
      cyclone();
      break;
    }
    delay(30);
  }
}

void cylon() {
  uint8_t hue = 75;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue, 255, 255);
    FastLED.show();
    fadeall();
    delay(10);
  }
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    leds[i] = CHSV(hue, 255, 255);
    FastLED.show();
    fadeall();
    delay(10);
  }
  hue++;
}

void explosion() {
  fadeall();
  for (int i = 0; i < NUM_LEDS / 2; i++) {
    leds[i] = CRGB::Amethyst;
    leds[i + 1] = CRGB::Amethyst;
    leds[i + 2] = CRGB::Amethyst;
    leds[i + 3] = CRGB::Amethyst;
    leds[NUM_LEDS - i] = CRGB::Aqua;
    leds[NUM_LEDS - i - 1] = CRGB::Aqua;
    leds[NUM_LEDS - i - 2] = CRGB::Aqua;
    leds[NUM_LEDS - i - 3] = CRGB::Aqua;

    FastLED.show();
    delay(80);
    leds[i]=CRGB::Black;
    leds[NUM_LEDS-i]=CRGB::Black;
    FastLED.show();
  }
  for (int i = 0; i < NUM_LEDS/2; i++) {
    if (i /22 != 1) {
      leds[25 + i] = CRGB::Aqua;
      leds[25 - i] = CRGB::Aqua;
      FastLED.show();
      delay(12);
    }else{
      leds[25 + i] = CRGB::Amethyst;
      leds[25 - i] = CRGB::Amethyst;
      FastLED.show();
      delay(12);
    }
  }
  for (int i = 255; i > 0; i--) {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(10);
  }
}




double getVoltage() {
  unsigned long startMillis = millis();  // Start of sample window
  unsigned int peakToPeak = 0;           // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect micropone data for 50 mS
  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(MIC_PIN);
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax) {
        signalMax = sample;  // save just the max levels
      } else if (sample < signalMin) {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;        // max - min = peak-peak amplitude
  double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
  return volts;
}
