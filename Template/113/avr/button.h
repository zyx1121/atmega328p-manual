#ifndef _BUTTON_H_
#define _BUTTON_H_

const uint8_t BTN_NUM = 4;
const uint8_t BTN_PIN[] = {10, 11, 12, 13};

enum {
  IDLE,
  PRESS,
  HOLD,
  RELEASE
};

typedef struct {
  uint8_t status;
  uint64_t holdStart;
  uint64_t holdTime;
} button_t;

button_t button[BTN_NUM] = {0, 0, 0};

void initButton(void) {
  for (uint8_t i = 0; i < BTN_NUM; i++) {
    pinMode(BTN_PIN[i], INPUT);
  }
}

void loopButton(void) {
  for (uint8_t i = 0; i < BTN_NUM; i++) {
    uint8_t read = digitalRead(BTN_PIN[i]);

    switch (button[i].status) {
      case IDLE:
        button[i].status = read ? button[i].status : PRESS;
        break;

      case PRESS:
        button[i].status = HOLD;
        button[i].holdStart = millis();
        break;

      case HOLD:
        button[i].status = read ? RELEASE : button[i].status;
        button[i].holdTime = millis() - button[i].holdStart;
        break;

      case RELEASE:
        button[i].status = IDLE;
        button[i].holdTime = 0;
        break;
    }
  }
}

#endif