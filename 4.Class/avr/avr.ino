/**
 * 按鈕控制 LED 練習 - 使用類的實現
 *
 * 目標：使用 LED 和 Button 類來實現按鈕控制 LED 的功能。
 *
 * 說明：
 * 1. 程式設置了 8 顆 LED（腳位 2-9）和 4 個按鈕（腳位 10-13）。
 * 2. LED 類管理 LED 的狀態和控制。
 * 3. Button 類處理按鈕的狀態檢測和更新。
 * 4. mainLoop() 函式實現了以下邏輯：
 *    - 按下或長按第一個按鈕（btn[0]）會使 LED 值加 1。
 *    - 按下或長按第二個按鈕（btn[1]）會使 LED 值減 1。
 *
 * 學習重點：
 * 1. 如何使用類來組織和管理相關的功能。
 * 2. 如何在主程式中使用自定義的類。
 * 3. 如何實現簡單的按鈕控制邏輯。
 *
 * ! 重要：觀察如何通過類的方法來訪問和控制硬體。
 *
 * ? 思考：這種使用類的方法相比直接在 main 文件中實現所有邏輯有什麼優勢？
 * ? 思考：如何擴展這個程式來支持更多的功能，例如不同的 LED 模式？
 *
 * TODO: 嘗試添加新的功能，如雙擊按鈕、長按時快速增減等。
 */

#include "led.h"
#include "button.h"

// 定義 LED 腳位和數量
const uint8_t LED_PINS[] = {2, 3, 4, 5, 6, 7, 8, 9};
const uint8_t LED_COUNT = sizeof(LED_PINS) / sizeof(LED_PINS[0]);

// 定義按鈕腳位和數量
const uint8_t BTN_PINS[] = {10, 11, 12, 13};
const uint8_t BTN_COUNT = sizeof(BTN_PINS) / sizeof(BTN_PINS[0]);

// 創建 LED 和 Button 對象
LED led(LED_PINS, LED_COUNT);
Button button(BTN_PINS, BTN_COUNT);

void setup() {
    led.setup();    // 初始化 LED
    button.setup(); // 初始化按鈕
}

void loop() {
    button.loop(); // 更新按鈕狀態
    led.loop();    // 更新 LED 狀態
    mainLoop();    // 處理主要邏輯
}

void mainLoop() {
    // 檢查第一個按鈕（增加 LED 值）
    if ((button.getState(0) == BTN_PRESS) ||
        (button.getState(0) == BTN_HOLD && button.getHoldTime(0) > 1000)) {
        uint8_t ledState = led.getState();
        ledState = ledState == 255 ? 0 : ledState + 1;  // 循環增加 LED 值
        led.setState(ledState);
    }

    // 檢查第二個按鈕（減少 LED 值）
    if ((button.getState(1) == BTN_PRESS) ||
        (button.getState(1) == BTN_HOLD && button.getHoldTime(1) > 1000)) {
        uint8_t ledState = led.getState();
        ledState = ledState == 0 ? 255 : ledState - 1;  // 循環減少 LED 值
        led.setState(ledState);
    }
}