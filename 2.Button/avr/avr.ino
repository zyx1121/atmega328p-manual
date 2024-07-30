/**
 * Button 控制驅動練習
 *
 * 目標：完成 btnSetup() 和 btnLoop() 函式，實現按鈕的初始化和狀態檢測，使用狀態機方法處理按鈕狀態。
 *
 * 說明：
 * 1. 程式設置了 8 顆 LED（腳位 2-9）和 4 個按鈕（腳位 10-13）。
 * 2. led 變數控制 8 顆 LED 的狀態，每個位元對應一顆 LED。
 * 3. btn 陣列存儲 4 個按鈕的狀態，使用 enum btnState 定義不同的按鈕狀態。
 * 4. mainLoop() 函式展示了如何使用按鈕狀態來控制 LED：
 *    - 按下第一個按鈕（btn[0]）會使 led 值加 1。
 *    - 放開第二個按鈕（btn[1]）會使 led 值減 1。
 * 5. 你的任務是完成 btnSetup() 和 btnLoop() 函式：
 *    - 在 btnSetup() 中初始化所有按鈕腳位。
 *    - 在 btnLoop() 中使用狀態機方法實現按鈕狀態的檢測和更新。
 *
 * ! 重要：使用狀態機來處理按鈕狀態，不允許使用 delay() 函數。
 *
 * ? 思考：如何設計狀態機來有效區分和處理按鈕的各種狀態（閒置、按下、持續按住、放開）？
 *
 * TODO: 完成 btnSetup() 和 btnLoop() 函式
 */

const uint8_t LED_PINS[] = {2, 3, 4, 5, 6, 7, 8, 9};  // 8 顆 LED 的腳位
const uint8_t LED_COUNT = sizeof(LED_PINS) / sizeof(LED_PINS[0]);

const uint8_t BTN_PINS[] = {10, 11, 12, 13};  // 4 顆按鈕的腳位
const uint8_t BTN_COUNT = sizeof(BTN_PINS) / sizeof(BTN_PINS[0]);

enum btnState {
    BTN_IDLE = 0,     // 按鈕閒置
    BTN_PRESS = 1,    // 按鈕被按下
    BTN_HOLD = 2,     // 按鈕被按住
    BTN_RELEASE = 3,  // 按鈕被放開
};

uint8_t led = 0b00000000;  // 控制 8 顆 LED 的狀態，每個位元對應一顆 LED
uint8_t btn[BTN_COUNT] = {BTN_IDLE};  // 4 顆按鈕的狀態

void mainLoop() {
    if (btn[0] == BTN_PRESS)
        led = led == 255 ? 0 : led + 1;  // 第一個按鈕按下時，LED 值加 1（若達到 255 則歸零）

    if (btn[1] == BTN_RELEASE)
        led = led == 0 ? 255 : led - 1;  // 第二個按鈕放開時，LED 值減 1（若達到 0 則變為 255）
}

void setup() {
    ledSetup();  // 初始化所有 LED
    btnSetup();  // 初始化所有按鈕
}

void loop() {
    btnLoop();   // 檢測和更新按鈕狀態
    ledLoop();   // 控制所有 LED 的開關
    mainLoop();  // 處理按鈕與 LED 的互動邏輯
}

void ledSetup(void) {
    for (uint8_t i = 0; i < LED_COUNT; i++)
        pinMode(LED_PINS[i], OUTPUT);  // 設置所有 LED 腳位為輸出模式
}

void ledLoop(void) {
    for (uint8_t i = 0; i < LED_COUNT; i++)
        digitalWrite(LED_PINS[i], (led >> i) & 1);  // 根據 led 變數的每個位元控制對應的 LED
}

void btnSetup(void) {
    // TODO: 初始化所有按鈕腳位
    // * 提示：將按鈕設置為輸入模式
}

void btnLoop(void) {
    // TODO: 使用狀態機方法檢測和更新每個按鈕的狀態
    // * 提示：
    // * 1. 為每個按鈕實現一個簡單的狀態機
    // * 2. 讀取每個按鈕的當前物理狀態
    // * 3. 根據當前物理狀態和按鈕的當前狀態(btn[i])，決定下一個狀態
    // * 4. 更新 btn 陣列中的狀態
    // * 5. 確保正確處理按鈕的閒置、按下、持續按住和放開的狀態轉換
    // ! 注意：不要使用 delay() 函數，這會阻塞程式執行
    // ? 思考：狀態機如何幫助你區分瞬時按下和長按的情況？
}