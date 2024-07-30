#include <EEPROM.h>
#include "bluetooth.h"
#include "button.h"
#include "led.h"
#include "oled.h"
#include "rgb.h"

enum {
    BTN_BACK,
    BTN_UP,
    BTN_DOWN,
    BTN_ENTER,
};

uint8_t status = 0;
uint8_t prevStatus = 0;

uint8_t rgbIndex = 0;

void status0() {
    rgbRainbow(100);

    if (button[BTN_ENTER].status == PRESS) {
        status = 1;
        rgbIndex = 0;
        rgbClear();
    }
}

void status1() {
    oled.clearDisplay();
    for (uint8_t i = 0; i < 8; i++) {
        if (i == 7 - rgbIndex) {
            oled.fillCircle(i * 16 + 8, 8, 7, WHITE);
        } else {
            oled.drawCircle(i * 16 + 8, 8, 7, WHITE);
        }
    }
    oled.display();

    if (button[BTN_UP].status == PRESS ||
       (button[BTN_UP].status == HOLD && button[BTN_UP].holdTime >= 1000)) {
        rgbIndex = rgbIndex == 7 ? 0 : rgbIndex + 1;
    }

    if (button[BTN_DOWN].status == PRESS ||
       (button[BTN_DOWN].status == HOLD && button[BTN_DOWN].holdTime >= 1000)) {
        rgbIndex = rgbIndex == 0 ? 7 : rgbIndex - 1;
    }

    if (button[BTN_ENTER].status == PRESS) {
        status = 2;
    }
}

void status2() {
    static bool colorSelected = false;
    static uint8_t colorIndex = 0;
    char buffer[4];

    oled.clearDisplay();
    oled.setTextSize(2);

    oled.setCursor(0, 0);
    oled.print("RGB ");
    sprintf(buffer, "%1d", rgbIndex);
    oled.println(buffer);

    if (colorIndex == 0) oled.print(">");
    oled.println("R");
    if (colorIndex == 1) oled.print(">");
    oled.println("G");
    if (colorIndex == 2) oled.print(">");
    oled.println("B");

    oled.setCursor(SCREEN_WIDTH - 48, 16);
    sprintf(buffer, "%3d", rgb[rgbIndex].r);
    oled.print(buffer);

    oled.setCursor(SCREEN_WIDTH - 48, 32);
    sprintf(buffer, "%3d", rgb[rgbIndex].g);
    oled.print(buffer);

    oled.setCursor(SCREEN_WIDTH - 48, 48);
    sprintf(buffer, "%3d", rgb[rgbIndex].b);
    oled.print(buffer);

    oled.display();

    if (button[BTN_UP].status == PRESS ||
       (button[BTN_UP].status == HOLD && button[BTN_UP].holdTime >= 1000)) {
        if (colorSelected) {
            switch (colorIndex) {
                case 0: rgb[rgbIndex].r++; break;
                case 1: rgb[rgbIndex].g++; break;
                case 2: rgb[rgbIndex].b++; break;
            }
        } else {
            colorIndex = colorIndex == 0 ? 2 : colorIndex - 1;
        }
    }

    if (button[BTN_DOWN].status == PRESS ||
       (button[BTN_DOWN].status == HOLD && button[BTN_DOWN].holdTime >= 1000)) {
        if (colorSelected) {
            switch (colorIndex) {
                case 0: rgb[rgbIndex].r--; break;
                case 1: rgb[rgbIndex].g--; break;
                case 2: rgb[rgbIndex].b--; break;
            }
        } else {
            colorIndex = colorIndex == 2 ? 0 : colorIndex + 1;
        }
    }

    if (button[BTN_ENTER].status == PRESS) {
        colorSelected = true;
    }

    if (button[BTN_BACK].status == PRESS) {
        if (colorSelected) {
            colorSelected = false;
        } else {
            status = 1;
        }
    }
}

void status3() {}

void loopStatus() {
    switch (status) {
        case 0:
            status0();
            break;
        case 1:
            status1();
            break;
        case 2:
            status2();
            break;
        case 3:
            status3();
            break;
    }

    if (status != prevStatus) {
        prevStatus = status;

        packet_t statusPacket;

        statusPacket.type = PACKET_TYPE_MODE;
        statusPacket.length = 1;
        statusPacket.data[0] = status;

        sendPacket(statusPacket);
    }
}

void setup() {
    initBluetooth();
    initButton();
    initLED();
    initOLED();
    initRGB();
}

void loop() {
    loopBluetooth();
    loopButton();
    loopLED();
    loopRGB();
    loopStatus();
}
