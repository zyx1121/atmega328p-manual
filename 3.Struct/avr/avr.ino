/**
 * 按鈕連續狀態邏輯練習
 *
 * 目標：擴展 mainLoop() 函式，在保留現有功能的基礎上實現按鈕的連續狀態邏輯。
 *
 * 說明：
 * 1. 程式已設置了 8 顆 LED（腳位 2-9）和 4 個按鈕（腳位 10-13）。
 * 2. Button 結構體包含 state 和 holdTime 兩個成員。
 * 3. btnSetup() 和 btnLoop() 函式已經實現了基本的按鈕狀態檢測。
 * 4. 現有的 mainLoop() 實現了基本的按下觸發邏輯。
 * 5. 你的任務是擴展 mainLoop() 函式，實現以下邏輯：
 *    - 保留當前按下即觸發的功能。
 *    - 增加持續按住按鈕時的連續觸發功能。
 *    - 連續觸發應在按住一定時間後開始，且速度應比初次按下時快。
 *
 * ! 重要：確保新增的連續觸發邏輯不影響原有的按下觸發功能。
 *
 * ? 思考：如何利用 Button 結構中的 holdTime 來實現連續觸發？
 * ? 思考：如何平衡初次觸發和連續觸發的體驗？
 *
 * TODO: 擴展 mainLoop() 函式以實現連續狀態邏輯
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

typedef struct {
    uint8_t state;
    uint64_t holdTime;
} button_t;

uint8_t led = 0b00000000;  // 控制 8 顆 LED 的狀態，每個位元對應一顆 LED
button_t btn[BTN_COUNT] = {{BTN_IDLE}};  // 4 顆按鈕的狀態

void mainLoop() {
    // 現有的按下觸發邏輯
    if (btn[0].state == BTN_PRESS)
        led = led == 255 ? 0 : led + 1;

    if (btn[1].state == BTN_PRESS)
        led = led == 0 ? 255 : led - 1;

    // TODO: 實現按鈕的連續狀態邏輯
    // * 提示：
    // * 1. 保留上面的按下觸發邏輯
    // * 2. 增加對 BTN_HOLD 狀態的處理
    // * 3. 使用 btn[i].holdTime 來決定何時開始連續觸發
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
        digitalWrite(LED_PINS[i], bitRead(led, i));  // 根據 led 變數的每個位元控制對應的 LED
}

void btnSetup(void) {
    for (uint8_t i = 0; i < BTN_COUNT; i++)
        pinMode(BTN_PINS[i], INPUT);  // 設置所有按鈕腳位為輸入模式
}

void btnLoop(void) {
    static uint64_t prevTime[BTN_COUNT] = {0};  // 每個按鈕的上一次按下時間

    for (uint8_t i = 0; i < BTN_COUNT; i++) {
        uint8_t state = digitalRead(BTN_PINS[i]);  // 讀取按鈕的當前物理狀態

        switch (btn[i].state) {
            case BTN_IDLE:
                btn[i].state = state == LOW ? BTN_PRESS : BTN_IDLE;  // 檢查按鈕是否被按下，進入按下狀態
                break;

            case BTN_PRESS:
                btn[i].state = BTN_HOLD;  // 按下後進入按住狀態
                prevTime[i] = millis();
                break;

            case BTN_HOLD:
                btn[i].state = state == LOW ? BTN_HOLD : BTN_RELEASE;  // 檢查按鈕是否被放開，進入放開狀態
                btn[i].holdTime = state == LOW ? millis() - prevTime[i] : 0;  // 計算按住時間
                break;

            case BTN_RELEASE:
                btn[i].state = BTN_IDLE;  // 放開後回到閒置狀態
                break;
        }
    }
}