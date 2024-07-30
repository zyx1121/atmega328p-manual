#ifndef _LED_H_
#define _LED_H_

#define LED_PIN 9

bool led = false;

void ledCPU() {
    static uint64_t prevMillis = millis();

    if (millis() - prevMillis >= 500) {
        prevMillis = millis();
        led = !led;
    }
}

void initLED() {
    pinMode(LED_PIN, OUTPUT);
}

void loopLED() {
    digitalWrite(LED_PIN, led);
    ledCPU();
}

#endif