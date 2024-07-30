#include "button.h"

Button::Button(const uint8_t* pins, uint8_t count) : btnPins(pins), btnCount(count) {
    buttons = new ButtonState[btnCount];  // 動態分配按鈕狀態數組
    prevTime = new uint64_t[btnCount];    // 動態分配時間記錄數組
    for (uint8_t i = 0; i < btnCount; i++) {
        buttons[i] = {BTN_IDLE, 0};  // 初始化每個按鈕的狀態為閒置
        prevTime[i] = 0;             // 初始化時間記錄
    }
}

Button::~Button() {
    delete[] buttons;   // 釋放動態分配的內存
    delete[] prevTime;
}

void Button::setup() {
    for (uint8_t i = 0; i < btnCount; i++) {
        pinMode(btnPins[i], INPUT);  // 設置每個按鈕腳位為輸入模式
    }
}

void Button::loop() {
    for (uint8_t i = 0; i < btnCount; i++) {
        uint8_t state = digitalRead(btnPins[i]);  // 讀取按鈕的當前物理狀態
        switch (buttons[i].state) {
            case BTN_IDLE:
                buttons[i].state = state == LOW ? BTN_PRESS : BTN_IDLE;  // 檢測按下
                break;
            case BTN_PRESS:
                buttons[i].state = BTN_HOLD;  // 按下後立即轉為按住狀態
                prevTime[i] = millis();       // 記錄按下的時間
                break;
            case BTN_HOLD:
                buttons[i].state = state == LOW ? BTN_HOLD : BTN_RELEASE;  // 檢測釋放
                buttons[i].holdTime = state == LOW ? millis() - prevTime[i] : 0;  // 計算按住時間
                break;
            case BTN_RELEASE:
                buttons[i].state = BTN_IDLE;  // 釋放後回到閒置狀態
                break;
        }
    }
}

btnState Button::getState(uint8_t index) const {
    return static_cast<btnState>(buttons[index].state);
}

uint64_t Button::getHoldTime(uint8_t index) const {
    return buttons[index].holdTime;
}