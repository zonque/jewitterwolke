#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define NUM_PIXEL 150
#define PIXEL_PIN 14
Adafruit_NeoPixel *strip;
unsigned char pixel[50];

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);

  strip = new Adafruit_NeoPixel(NUM_PIXEL, PIXEL_PIN, NEO_GRB | NEO_KHZ800);
  strip->begin();

  for (unsigned int i = 0; i < NUM_PIXEL; i++) {
    strip->setPixelColor(i, 0, 0, 0);
  }

  strip->show();

  while (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    delay(1000);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void pixel_sync() {
  int stride = NUM_PIXEL / sizeof(pixel);

  for (unsigned int i = 0; i < sizeof(pixel); i++) {
    strip->setPixelColor(i * stride, pixel[i], pixel[i], pixel[i]);
  }

  strip->show();
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

void jump_check(int n) {
  const int threshold = 3.0;

  for (int i = 0; i < n; i++) {
    Serial.println("checking ...");
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    if (g.gyro.x > threshold || g.gyro.y > threshold || g.gyro.z > threshold)
      flash(random(50) + 10);

    yield();
  }
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
    jump_check(15);
  }

  memset(pixel, 0, sizeof(pixel));
  pixel_sync();
}

void loop() {
  lightning(random(sizeof(pixel)), random(15));

  if (random(5) == 0) {
    short_flashes(random(3));
  }

  jump_check(500);
}