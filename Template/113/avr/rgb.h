#ifndef _RGB_H_
#define _RGB_H_

#include <Adafruit_NeoPixel.h>
#include "bluetooth.h"

#define RGB_NUM 8
#define RGB_PIN 8

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} rgb_t;

Adafruit_NeoPixel pixels(RGB_NUM, RGB_PIN, NEO_GRB + NEO_KHZ800);

rgb_t rgb[RGB_NUM];
rgb_t prevRGB[RGB_NUM];

void rgbClear() {
  for (uint8_t i = 0; i < RGB_NUM; i++) {
    rgb[i].r = 0;
    rgb[i].g = 0;
    rgb[i].b = 0;
  }
}

void rgbWipe(rgb_t color, uint8_t wait) {
  static uint64_t prevMillis = millis();
  static uint8_t i = 0;

  if (millis() - prevMillis > wait) {
    prevMillis = millis();

    rgb[i] = color;
    i = i == RGB_NUM - 1 ? 0 : i + 1;
  }
}

void rgbRainbow(uint8_t wait) {
  static uint64_t prevMillis = millis();
  static uint8_t j = 0;

  if (millis() - prevMillis > wait) {
    prevMillis = millis();

    for (uint8_t i = 0; i < RGB_NUM; i++) {
      rgb[i].r = (uint8_t)((1 - cos((float)(i + j) * 2 * PI / RGB_NUM)) * 127.5);
      rgb[i].g = (uint8_t)((1 - cos((float)(i + j) * 2 * PI / RGB_NUM - 2 * PI / 3)) * 127.5);
      rgb[i].b = (uint8_t)((1 - cos((float)(i + j) * 2 * PI / RGB_NUM - 4 * PI / 3)) * 127.5);
    }

    j = j == 255 ? 0 : j + 1;
  }
}

void initRGB() {
  pixels.begin();
  pixels.setBrightness(10);
  pixels.clear();

  for (uint8_t i = 0; i < RGB_NUM; i++) {
    rgb[i].r = 0;
    rgb[i].g = 0;
    rgb[i].b = 0;
    prevRGB[i] = rgb[i];
  }
}

void loopRGB() {
  bool needUpdate = false;

  for (uint8_t i = 0; i < RGB_NUM; i++) {
    if (rgb[i].r != prevRGB[i].r || rgb[i].g != prevRGB[i].g || rgb[i].b != prevRGB[i].b) {
      prevRGB[i] = rgb[i];
      needUpdate = true;

      pixels.setPixelColor(i, pixels.Color(rgb[i].r, rgb[i].g, rgb[i].b));

      packet_t rgbPacket;

      rgbPacket.type = PACKET_TYPE_RGB;
      rgbPacket.length = 4;
      rgbPacket.data[0] = i;
      rgbPacket.data[1] = rgb[i].r;
      rgbPacket.data[2] = rgb[i].g;
      rgbPacket.data[3] = rgb[i].b;

      sendPacket(rgbPacket);
    }
  }

  if (needUpdate) {
    pixels.show();
  }
}

#endif // _RGB_H_