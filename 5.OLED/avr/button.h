#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

/**
 * @enum btnState
 * @brief 定義按鈕的不同狀態
 */
enum btnState {
    BTN_IDLE = 0,     // 按鈕閒置
    BTN_PRESS = 1,    // 按鈕被按下
    BTN_HOLD = 2,     // 按鈕被按住
    BTN_RELEASE = 3,  // 按鈕被放開
};

/**
 * @class Button
 * @brief 管理多個按鈕的類
 *
 * 這個類封裝了多個按鈕的狀態檢測邏輯,實現了一個簡單的狀態機來跟踪每個按鈕的狀態。
 */
class Button {
private:
    const uint8_t* btnPins;  // 按鈕腳位數組
    uint8_t btnCount;        // 按鈕的數量

    struct ButtonState {
        uint8_t state;     // 當前按鈕狀態
        uint64_t holdTime; // 按鈕被按住的時間
    };

    ButtonState* buttons;  // 動態分配的按鈕狀態數組
    uint64_t* prevTime;    // 動態分配的時間記錄數組

public:
    /**
     * @brief 構造函數
     * @param pins 按鈕腳位數組
     * @param count 按鈕的數量
     */
    Button(const uint8_t* pins, uint8_t count);

    /**
     * @brief 析構函數
     * 釋放動態分配的內存
     */
    ~Button();

    /**
     * @brief 初始化按鈕腳位
     * 將所有按鈕腳位設置為輸入模式
     */
    void setup();

    /**
     * @brief 更新按鈕狀態
     * 檢測每個按鈕的狀態並更新狀態機
     */
    void loop();

    /**
     * @brief 獲取指定按鈕的狀態
     * @param index 按鈕索引
     * @return 按鈕的當前狀態
     */
    btnState getState(uint8_t index) const;

    /**
     * @brief 獲取指定按鈕被按住的時間
     * @param index 按鈕索引
     * @return 按鈕被按住的時間（毫秒）
     */
    uint64_t getHoldTime(uint8_t index) const;
};

#endif // BUTTON_H