/**
 * 綜合應用：LED、按鈕、OLED 顯示和 RTC 時鐘
 *
 * 目標：整合 LED 控制、按鈕輸入、OLED 顯示和 RTC 時鐘功能
 *
 * 說明：
 * 1. 使用自定義的 LED 和 Button 類控制 LED 和檢測按鈕狀態
 * 2. 使用 Adafruit_SSD1306 控制 OLED 顯示
 * 3. 使用 TimeManager 類管理 RTC 功能
 * 4. 程序功能：
 *    - 顯示當前時間和日期
 *    - 通過按鈕切換顯示模式和設置時間
 *    - OLED 顯示時間信息和設置狀態
 *
 * 學習重點：
 * 1. 如何整合多個硬件組件和軟件模塊
 * 2. 如何實現一個簡單的狀態機來管理不同的操作模式
 * 3. 如何使用 RTC 來獲取和設置時間
 *
 * ! 重要：確保已安裝 Adafruit SSD1306、Adafruit GFX Library 和 RTClib
 *
 * ? 思考：如何優化用戶界面以使時間設置更加直觀？
 * ? 思考：如何擴展這個系統以包含鬧鐘或計時器功能？
 *
 * TODO: 實現長按按鈕快速增減數值的功能
 * TODO: 添加更多顯示模式，如顯示星期幾或農曆日期
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Button.h"
#include "Time.h"
#include "LED.h"

// OLED 顯示器設置
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

// LED 和按鈕的腳位設置
const uint8_t LED_PINS[] = {2, 3, 4, 5, 6, 7, 8, 9};
const uint8_t LED_COUNT = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
const uint8_t BTN_PINS[] = {10, 11, 12, 13};
const uint8_t BTN_COUNT = sizeof(BTN_PINS) / sizeof(BTN_PINS[0]);

// 創建 LED、Button、OLED 和 TimeManager 對象
LED led(LED_PINS, LED_COUNT);
Button button(BTN_PINS, BTN_COUNT);
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
TimeManager timeManager;

// 系統狀態枚舉
enum systemState {
    DISPLAY_TIME,  // 顯示時間模式
    SET_MODE,      // 設置時間模式
};

// 設置時間的不同階段
enum setState {
    SET_YEAR,
    SET_MONTH,
    SET_DAY,
    SET_HOUR,
    SET_MINUTE,
    SET_SECOND,
};

uint8_t systemState = DISPLAY_TIME;  // 初始系統狀態

void setup() {
    Serial.begin(9600);
    button.setup();
    led.setup();

    // 初始化 RTC
    if (!timeManager.begin()) {
        Serial.println(F("Couldn't find RTC"));
        while (1);  // 如果 RTC 初始化失敗，程序停止
    }

    // 初始化 OLED 顯示器
    if(!oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        while (1);  // 如果 OLED 初始化失敗，程序停止
    }

    // 顯示啟動信息
    oled.clearDisplay();
    oled.setTextSize(2);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(0,0);
    oled.println(F("RTC Clock"));
    oled.display();
    delay(2000);
}

void loop() {
    led.loop();
    button.loop();
    timeManager.update();

    // 根據當前系統狀態執行相應的功能
    switch (systemState) {
        case DISPLAY_TIME:
            stateDisplayTime();
            break;
        case SET_MODE:
            stateSetMode();
            break;
    }
}

// 顯示時間的狀態
void stateDisplayTime() {
    DateTime currentTime = timeManager.getCurrentTime();

    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(0,0);
    oled.println(F("Current Time:"));

    // 長按按鈕 1 顯示日期，否則顯示時間
    if (button.getState(1) == BTN_HOLD) {
        oled.print(currentTime.year());
        oled.print(F("-"));
        oled.print(currentTime.month());
        oled.print(F("-"));
        oled.print(currentTime.day());
    } else {
        oled.print(currentTime.hour());
        oled.print(timeManager.isInSecondHalf() ? F(":") : F(" "));
        oled.print(currentTime.minute());
        oled.print(timeManager.isInSecondHalf() ? F(":") : F(" "));
        oled.println(currentTime.second());
    }

    oled.display();

    // 按下按鈕 0 進入設置模式
    if (button.getState(0) == BTN_PRESS) {
        systemState = SET_MODE;
    }
}

// 設置時間的狀態
void stateSetMode() {
    static uint8_t setState = SET_HOUR;  // 靜態變量保存當前設置的項目

    DateTime currentTime = timeManager.getCurrentTime();
    DateTime newTime = currentTime;

    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(0, 0);

    // 根據當前設置項目顯示相應信息
    switch (setState) {
        case SET_YEAR:
            oled.println(F("Set Year:"));
            oled.print(newTime.year());
            break;
        case SET_MONTH:
            oled.println(F("Set Month:"));
            oled.print(newTime.month());
            break;
        case SET_DAY:
            oled.println(F("Set Day:"));
            oled.print(newTime.day());
            break;
        case SET_HOUR:
            oled.println(F("Set Hour:"));
            oled.print(newTime.hour());
            break;
        case SET_MINUTE:
            oled.println(F("Set Minute:"));
            oled.print(newTime.minute());
            break;
        case SET_SECOND:
            oled.println(F("Set Second:"));
            oled.print(newTime.second());
            break;
    }

    oled.display();

    // 按鈕 1 增加值
    if (button.getState(1) == BTN_PRESS) {
        newTime = newTime + TimeSpan(0, 1, 0, 0);
    }

    // 按鈕 2 減少值
    if (button.getState(2) == BTN_PRESS) {
        newTime = newTime - TimeSpan(0, 1, 0, 0);
    }

    // 按鈕 0 切換設置項目或完成設置
    if (button.getState(0) == BTN_PRESS) {
        setState = setState == SET_SECOND ? SET_YEAR : (setState + 1);

        if (setState == SET_YEAR) {
            systemState = DISPLAY_TIME;  // 完成設置，返回顯示時間模式
            timeManager.setTime(newTime);  // 更新 RTC 時間
        }
    }
}