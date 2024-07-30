#include "BluetoothManager.h"

BluetoothManager::BluetoothManager(SoftwareSerial& serial)
    : btSerial(serial), bufferHead(0), bufferTail(0), bufferFull(false) {}

void BluetoothManager::begin(long baud) {
    btSerial.begin(baud);
}

void BluetoothManager::update() {
    while (btSerial.available() >= 4 && !bufferFull) {
        if (!parsePacket()) {
            btSerial.read(); // 解析失敗時丟棄一個字節
        }
    }
}

bool BluetoothManager::parsePacket() {
    if (btSerial.read() != PACKET_HEADER) return false;

    Packet tempPacket;
    tempPacket.type = btSerial.read();
    tempPacket.length = btSerial.read();

    if (tempPacket.length > MAX_PACKET_SIZE) return false;

    for (int i = 0; i < tempPacket.length; i++) {
        tempPacket.data[i] = btSerial.read();
    }

    if (btSerial.read() != PACKET_FOOTER) return false;

    return addToBuffer(tempPacket);
}

bool BluetoothManager::addToBuffer(const Packet& packet) {
    if (bufferFull) return false;

    packetBuffer[bufferTail] = packet;
    bufferTail = (bufferTail + 1) % PACKET_BUFFER_SIZE;
    bufferFull = (bufferHead == bufferTail);

    return true;
}

bool BluetoothManager::isPacketAvailable() const {
    return bufferHead != bufferTail || bufferFull;
}

bool BluetoothManager::getPacket(Packet& packet) {
    if (!isPacketAvailable()) return false;

    packet = packetBuffer[bufferHead];
    bufferHead = (bufferHead + 1) % PACKET_BUFFER_SIZE;
    bufferFull = false;

    return true;
}

void BluetoothManager::sendPacket(uint8_t type, const uint8_t* data, uint8_t length) {
    btSerial.write(PACKET_HEADER);
    btSerial.write(type);
    btSerial.write(length);
    btSerial.write(data, length);
    btSerial.write(PACKET_FOOTER);
}