#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define NUM_PIXEL 150
#define PIXEL_PIN 14
Adafruit_NeoPixel *strip;

unsigned char pixel[50];

void setup() {
  strip = new Adafruit_NeoPixel(NUM_PIXEL, PIXEL_PIN, NEO_GRB | NEO_KHZ800);
  strip->begin();

  for (unsigned int i = 0; i < NUM_PIXEL; i++) {
    strip->setPixelColor(i, 0, 0, 0);
  }

  strip->show();
}

void pixel_sync() {
  int stride = NUM_PIXEL / sizeof(pixel);

  for (unsigned int i = 0; i < sizeof(pixel); i++) {
    strip->setPixelColor(i * stride, pixel[i], pixel[i], pixel[i]);
  }

  strip->show();
}

void lightning(unsigned int center, unsigned int size) {
  for (unsigned int n = 0; n < size*2; n++) {
    memset(pixel, 0, sizeof(pixel));

    pixel[center] = random(255);

    for (int i = 0; i < size/2; i++) {
      if (center+i < sizeof(pixel))
        pixel[center+i] = random(255); //min(0, int(pixel[center] - random(100)));

      if (center-i >= 0)
        pixel[center-i] = random(255); //min(0, int(pixel[center] - random(100)));
    }

    pixel_sync();
    delay(50);
  }

  memset(pixel, 0, sizeof(pixel));
  pixel_sync();
}

void flash(int len) {
  memset(pixel, 0, sizeof(pixel));

  for (unsigned int i = 0; i < sizeof(pixel); i++) {
    pixel[i] = 127 + random(127);
    pixel_sync();
  }

  for (int i = 0; i < len; i++) {
    memset(pixel, 0, sizeof(pixel));
    pixel_sync();
    delay(random(20));

    memset(pixel, 127 + random(127), sizeof(pixel));
    pixel_sync();
    delay(random(20));
  }

  for (unsigned int i = 0; i < sizeof(pixel); i++) {
    pixel[i] = 0;
    pixel_sync();
  }
}

void short_flashes(int n) {
  for (int i = 0; i < n; i++) {
    delay(100);
    memset(pixel, 127 + random(127), sizeof(pixel));
    pixel_sync();
    delay(random(20));

    memset(pixel, 0, sizeof(pixel));
    pixel_sync();
  }
}

void loop() {
  lightning(random(sizeof(pixel)), random(15));

  if (random(5) == 0) {
    short_flashes(random(3));
  }

  if (random(15) == 0) {
    flash(random(50) + 10);
  }

  delay(random(3000));
}