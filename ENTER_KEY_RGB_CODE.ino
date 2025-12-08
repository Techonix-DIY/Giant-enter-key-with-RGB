#include <FastLED.h>

#define LED_PIN     D1
#define BUTTON_PIN  D2
#define NUM_LEDS    20
#define BRIGHTNESS  200
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

bool lastButtonState = HIGH;
bool buttonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  Serial.println("Ready. Press button for warm color animation.");
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        Serial.println("Button pressed -> start warm gradient animation");
        runWarmGradientAnimation();
      }
    }
  }

  lastButtonState = reading;
}

void runWarmGradientAnimation() {
  const int tail = 2; // تعداد پیکسل‌های دنباله

  for (int i = 0; i < NUM_LEDS + tail; i++) {
    FastLED.clear();

    // hue از نارنجی (حدود 25) تا قرمز (حدود 0)
    uint8_t hue = map(i, 0, NUM_LEDS, 25, 0);
    // اشباع از 180 تا 255 (هرچه جلوتر می‌ره، رنگ گرم‌تر می‌شه)
    uint8_t sat = map(i, 0, NUM_LEDS, 180, 255);

    CRGB color = CHSV(hue, sat, 255); // روشنایی ثابت = 255

    for (int j = 0; j < tail; j++) {
      int index = i - j;
      if (index >= 0 && index < NUM_LEDS) {
        leds[index] = color;
      }
    }

    FastLED.show();
    delay(30); // سرعت حرکت
  }

  FastLED.clear();
  FastLED.show();
}
