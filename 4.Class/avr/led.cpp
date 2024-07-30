#include "led.h"

LED::LED(const uint8_t* pins, uint8_t count) : ledPins(pins), ledCount(count), ledState(0) {
    // 初始化 LED 狀態為全關
}

void LED::setup() {
    for (uint8_t i = 0; i < ledCount; i++) {
        pinMode(ledPins[i], OUTPUT);  // 設置每個 LED 腳位為輸出模式
    }
}

void LED::loop() {
    for (uint8_t i = 0; i < ledCount; i++) {
        // 使用 bitRead 函數來讀取 ledState 的每一位,並據此設置 LED 的狀態
        digitalWrite(ledPins[i], bitRead(ledState, i));
    }
}

void LED::setState(uint8_t state) {
    ledState = state;  // 更新 LED 狀態
}

uint8_t LED::getState() const {
    return ledState;  // 返回當前 LED 狀態
}