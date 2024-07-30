/**
 * LED 控制驅動練習
 *
 * 目標：完成 ledSetup() 和 ledLoop() 函式，實現 8 顆 LED 的控制。
 *
 * 說明：
 * 1. LED_PINS 陣列包含了 8 個 LED 的腳位編號。
 * 2. mainLoop() 函式每秒更新一次 led 變數，產生不同的 LED 模式。
 * 3. 你的任務是完成 ledSetup() 和 ledLoop() 函式：
 *    - 在 ledSetup() 中將所有 LED_PINS 設置為輸出模式。
 *    - 在 ledLoop() 中根據 led 變數的每一個位元來控制對應的 LED。
 * 4. 不要修改其他函式的內容。
 *
 * ! 重要：確保正確使用位元運算來控制個別 LED。
 *
 * ? 思考：如何高效地遍歷 led 變數的每一個位元？
 *
 * TODO: 完成 ledSetup() 和 ledLoop() 函式
 */

const uint8_t LED_PINS[] = {2, 3, 4, 5, 6, 7, 8, 9};  // 8 顆 LED 的腳位
const uint8_t LED_COUNT = sizeof(LED_PINS) / sizeof(LED_PINS[0]);

uint8_t led = 0b00000000;  // 控制 8 顆 LED 的狀態，每個位元對應一顆 LED

void mainLoop() {
  static uint64_t prevTime = millis();

  // 每隔 1000 毫秒（1秒）更新 LED 狀態
  if (millis() - prevTime > 1000) {
    prevTime = millis();

    // 更新 led 變數，產生不同的 LED 模式
    // * 這裡使用一個簡單的遞增，你也可以實現其他有趣的模式
    led = (led + 1) % 256;
  }
}

void setup() {
  ledSetup();  // 初始化所有 LED
}

void loop() {
  ledLoop();   // 控制所有 LED 的開關
  mainLoop();  // 處理 LED 模式的邏輯
}

void ledSetup(void) {
  // TODO: 在這裡將所有 LED_PINS 初始化為輸出模式
  // * 提示：使用 for 迴圈來初始化所有的 LED
}

void ledLoop(void) {
  // TODO: 在這裡根據 led 變數的每一個位元來控制對應的 LED
  // * 提示：
  // * 1. 使用位元運算 (如 &) 來檢查 led 變數的每一個位元
  // * 2. 記得 LED_PINS[0] 對應 led 變數的最低位（最右邊的位元）
  // ? 思考：如何使用位移運算來簡化位元檢查？
}