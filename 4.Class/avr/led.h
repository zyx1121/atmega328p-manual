#ifndef LED_H
#define LED_H

#include <Arduino.h>

/**
 * @class LED
 * @brief 管理多個 LED 的類
 *
 * 這個類封裝了多個 LED 的控制邏輯,允許用戶設置 LED 的狀態並自動更新 LED 的輸出。
 */
class LED {
private:
    const uint8_t* ledPins;  // LED 腳位數組
    uint8_t ledCount;        // LED 的數量
    uint8_t ledState;        // 當前 LED 狀態 (每個位對應一個 LED)

public:
    /**
     * @brief 構造函數
     * @param pins LED 腳位數組
     * @param count LED 的數量
     */
    LED(const uint8_t* pins, uint8_t count);

    /**
     * @brief 初始化 LED 腳位
     * 將所有 LED 腳位設置為輸出模式
     */
    void setup();

    /**
     * @brief 更新 LED 狀態
     * 根據當前的 ledState 更新所有 LED 的輸出
     */
    void loop();

    /**
     * @brief 設置 LED 狀態
     * @param state 新的 LED 狀態
     */
    void setState(uint8_t state);

    /**
     * @brief 獲取當前 LED 狀態
     * @return 當前的 LED 狀態
     */
    uint8_t getState() const;
};

#endif // LED_H