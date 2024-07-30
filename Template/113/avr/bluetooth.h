#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <EEPROM.h>

#define ADDR1 0
#define ADDR2 1
#define ADDR3 3

#define BAUD_RATE 9600
#define MIN_PACKET_SIZE 3
#define MAX_PACKET_SIZE 16
#define PACKET_HEADER 0xAA

enum {
  PACKET_TYPE_RGB,
  PACKET_TYPE_MODE,
  PACKET_EEPROM_WRITE,
  PACKET_EEPROM_READ,
};

typedef struct {
  uint8_t type;
  uint8_t length;
  uint8_t data[MAX_PACKET_SIZE];
} packet_t;

void processPacket(const packet_t& packet);
void sendPacket(const packet_t& packet);
void parsePacket();
void initBluetooth(uint64_t baud);
void loopBluetooth();

#include "button.h"
#include "rgb.h"

extern uint8_t status;

void sendPacket(const packet_t& packet) {
  Serial.write(PACKET_HEADER);
  Serial.write(packet.type);
  Serial.write(packet.length);
  for (uint8_t i = 0; i < packet.length; i++) {
    Serial.write(packet.data[i]);
  }
}

void processPacket(const packet_t& packet) {
  switch (packet.type) {
    case PACKET_TYPE_RGB:
      if (packet.length == 4) {
        uint8_t index = packet.data[0];
        rgb[index].r = packet.data[1];
        rgb[index].g = packet.data[2];
        rgb[index].b = packet.data[3];
      }
      break;

    case PACKET_TYPE_MODE:
      if (packet.length == 1) {
        status = packet.data[0];
      }
      break;

    case PACKET_EEPROM_WRITE:
      rgb[0].r = 255;
      rgb[0].g = 255;
      rgb[0].b = 255;
      if (packet.length == 3) {
        EEPROM.update(ADDR1, packet.data[0]);
        EEPROM.update(ADDR2, packet.data[1]);
        EEPROM.update(ADDR3, packet.data[2]);
      }
      break;

    case PACKET_EEPROM_READ:
      rgb[1].r = 255;
      rgb[1].g = 255;
      rgb[1].b = 255;
      if (packet.length == 1) {
        packet_t eepromPacket;

        eepromPacket.type = PACKET_EEPROM_READ;
        eepromPacket.length = 3;
        eepromPacket.data[0] = EEPROM.read(ADDR1);
        eepromPacket.data[1] = EEPROM.read(ADDR2);
        eepromPacket.data[2] = EEPROM.read(ADDR3);

        sendPacket(eepromPacket);
      }
      break;
  }
}

void parsePacket() {
  packet_t tempPacket;

  if (Serial.read() != PACKET_HEADER) return;

  if (Serial.readBytes(&tempPacket.type, 1) != 1) return;

  if (Serial.readBytes(&tempPacket.length, 1) != 1) return;

  if (Serial.readBytes(tempPacket.data, tempPacket.length) != tempPacket.length) return;

  processPacket(tempPacket);
}

void initBluetooth() {
  Serial.begin(BAUD_RATE);
  Serial.setTimeout(10);
}

void loopBluetooth() {
  if (Serial.available()) {
    parsePacket();
  }
}

#endif