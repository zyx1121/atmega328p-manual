/**
 * 綜合應用：LED、按鈕和 OLED 顯示
 *
 * 目標：整合 LED 和按鈕控制，並直接使用 Adafruit_SSD1306 庫來控制 OLED 顯示
 *
 * 說明：
 * 1. 使用自定義的 LED 和 Button 類來控制 LED 和檢測按鈕狀態
 * 2. 直接使用 Adafruit_SSD1306 庫來控制 OLED 顯示
 * 3. 程序功能：
 *    - 按鈕 0 增加 LED 亮度
 *    - 按鈕 1 減少 LED 亮度
 *    - OLED 顯示當前 LED 亮度值和圖形表示
 *
 * 學習重點：
 * 1. 如何直接在項目中使用第三方庫（Adafruit_SSD1306）
 * 2. 如何結合自定義類和第三方庫來創建完整的應用
 * 3. 如何使用 OLED 顯示來提供視覺反饋
 *
 * ! 重要：確保已安裝 Adafruit SSD1306 和 Adafruit GFX Library
 *
 * ? 思考：直接使用第三方庫與封裝成自定義類各有什麼優缺點？
 * ? 思考：如何平衡代碼的簡潔性和功能的豐富性？
 *
 * TODO: 嘗試使用 Adafruit_SSD1306 庫的其他高級功能
 * TODO: 實現不同的顯示模式，並通過按鈕組合來切換
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "led.h"
#include "button.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

const uint8_t LED_PINS[] = {2, 3, 4, 5, 6, 7, 8, 9};
const uint8_t LED_COUNT = sizeof(LED_PINS) / sizeof(LED_PINS[0]);

const uint8_t BTN_PINS[] = {10, 11, 12, 13};
const uint8_t BTN_COUNT = sizeof(BTN_PINS) / sizeof(BTN_PINS[0]);

LED led(LED_PINS, LED_COUNT);
Button button(BTN_PINS, BTN_COUNT);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
    Serial.begin(9600);
    led.setup();
    button.setup();

    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println(F("LED Control"));
    display.display();
    delay(2000);
}

void loop() {
    button.loop();
    led.loop();
    updateLED();
    updateOLED();
}

void updateLED() {
    if (button.getState(0) == BTN_PRESS ||
        (button.getState(0) == BTN_HOLD && button.getHoldTime(0) > 1000)) {
        uint8_t ledState = led.getState();
        ledState = ledState == 255 ? 0 : ledState + 1;
        led.setState(ledState);
    }

    if (button.getState(1) == BTN_PRESS ||
        (button.getState(1) == BTN_HOLD && button.getHoldTime(1) > 1000)) {
        uint8_t ledState = led.getState();
        ledState = ledState == 0 ? 255 : ledState - 1;
        led.setState(ledState);
    }
}

void updateOLED() {
    static uint8_t lastLedState = 0;
    uint8_t currentLedState = led.getState();

    if (currentLedState != lastLedState) {
        display.clearDisplay();

        // 顯示文字
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0,0);
        display.print(F("LED Brightness: "));
        display.println(currentLedState);

        // 繪製亮度條
        int barWidth = map(currentLedState, 0, 255, 0, SCREEN_WIDTH);
        display.fillRect(0, 20, barWidth, 10, SSD1306_WHITE);
        display.drawRect(0, 20, SCREEN_WIDTH, 10, SSD1306_WHITE);

        // 繪製圓形指示器
        int circleX = map(currentLedState, 0, 255, 5, SCREEN_WIDTH-5);
        display.fillCircle(circleX, 45, 5, SSD1306_WHITE);

        display.display();
        lastLedState = currentLedState;
    }
}