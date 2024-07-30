#ifndef _OLED_H_
#define _OLED_H_

#include <Adafruit_SSD1306.h>
// #include "font.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// const unsigned char* getBitmapForChar(const char* c, uint8_t& width, uint8_t& height) {
//   for (CharBitmap& cb : charBitmaps) {
//     if (strcmp(cb.character, c) == 0) {
//       width = cb.width;
//       height = cb.height;
//       return cb.bitmap;
//     }
//   }
//   return nullptr;
// }

// void oledPrint(const char* str, int startX, int startY) {
//   int x = startX;
//   int y = startY;
//   char utf8_char[4] = {0};  // 用來保存單個UTF-8字符的字符串表示

//   while (*str) {
//     uint32_t codepoint = 0;
//     int bytes = 0;

//     // UTF-8解碼
//     if ((*str & 0x80) == 0) {
//       // 1字節（ASCII）
//       codepoint = *str;
//       bytes = 1;

//       // 使用內建函數打印ASCII字符
//       oled.setCursor(x, y);
//       oled.print((char)codepoint);
//       x = oled.getCursorX();  // 更新x座標
//     } else if ((*str & 0xE0) == 0xC0) {
//       // 2字節
//       codepoint = (*str & 0x1F) << 6;
//       codepoint |= (*(str + 1) & 0x3F);
//       bytes = 2;
//     } else if ((*str & 0xF0) == 0xE0) {
//       // 3字節
//       codepoint = (*str & 0x0F) << 12;
//       codepoint |= (*(str + 1) & 0x3F) << 6;
//       codepoint |= (*(str + 2) & 0x3F);
//       bytes = 3;
//     } else {
//       // 其他情況（暫不支持）
//       str++;
//       continue;
//     }

//     if (bytes > 1) {
//       // 複製UTF-8字符到 utf8_char
//       strncpy(utf8_char, str, bytes);
//       utf8_char[bytes] = '\0';  // 確保是以null結尾的字符串

//       uint8_t width, height;
//       const unsigned char* bitmap = getBitmapForChar(utf8_char, width, height);
//       if (bitmap) {
//         oled.drawBitmap(x, y, bitmap, width, height, SSD1306_WHITE);
//         x += width;
//         if (x + width > SCREEN_WIDTH) {
//           x = 0;
//           y += height;
//           if (y + height > SCREEN_HEIGHT) {
//             break;  // 畫面已滿
//           }
//         }
//       }
//     }
//     str += bytes;
//   }
//   oled.display();
// }

void initOLED() {
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.setTextColor(WHITE);
  oled.setTextSize(1);
  oled.setCursor(0, 0);
  oled.clearDisplay();
  oled.print("Hello, World!");
  // oledPrint("哈嘍世界", 0, 0);
  oled.display();
}

#endif  // _OLED_H_