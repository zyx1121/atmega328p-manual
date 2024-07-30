#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "BluetoothManager.h"
#include "Button.h"
#include "LED.h"
#include "Time.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

const uint8_t LED_PINS[] = {2, 3, 4, 5, 6, 7, 8, 9};
const uint8_t LED_COUNT = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
const uint8_t BTN_PINS[] = {10, 11, 12, 13};
const uint8_t BTN_COUNT = sizeof(BTN_PINS) / sizeof(BTN_PINS[0]);

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
LED led(LED_PINS, LED_COUNT);
Button btn(BTN_PINS, BTN_COUNT);
TimeManager timeManager;
SoftwareSerial bluetoothSerial(10, 11);
BluetoothManager btManager(bluetoothSerial);

void setup() {
    btn.setup();
    led.setup();
    timeManager.begin();
    btManager.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    btn.loop();
    led.loop();
    timeManager.update();
    btManager.update();

    Packet receivedPacket;
    while (btManager.getPacket(receivedPacket)) {
        processPacket(receivedPacket);
    }
}

void processPacket(const Packet& packet) {
    switch (packet.type) {
        case 0x01:  // LED control
            if (packet.length == 1) {
                led.setState(packet.data[0]);
            }
            break;
        case 0x02:  // Time control
            if (packet.length == 4) {
                uint32_t newTime = (packet.data[0] << 24) | (packet.data[1] << 16) | (packet.data[2] << 8) | packet.data[3];
                timeManager.setTime(newTime);
            }
            break;
        default:
            break;
    }
}