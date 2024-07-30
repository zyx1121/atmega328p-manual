#ifndef BLUETOOTH_MANAGER_H
#define BLUETOOTH_MANAGER_H

#include <Arduino.h>
#include <SoftwareSerial.h>

#define MAX_PACKET_SIZE 32
#define PACKET_BUFFER_SIZE 8
#define PACKET_HEADER 0x02
#define PACKET_FOOTER 0x03

struct Packet {
    uint8_t type;
    uint8_t length;
    uint8_t data[MAX_PACKET_SIZE];
};

/**
 * @brief 管理藍牙通信的類
 *
 * 這個類處理藍牙封包的接收、解析和發送。
 * 它使用循環緩衝區來存儲接收到的封包，以防止數據丟失。
 */
class BluetoothManager {
private:
    SoftwareSerial& btSerial;
    Packet packetBuffer[PACKET_BUFFER_SIZE];
    uint8_t bufferHead;
    uint8_t bufferTail;
    bool bufferFull;

    bool parsePacket();
    bool addToBuffer(const Packet& packet);

public:
    /**
     * @brief 構造函數
     * @param serial SoftwareSerial 對象的引用
     */
    BluetoothManager(SoftwareSerial& serial);

    /**
     * @brief 初始化藍牙串口
     * @param baud 波特率
     */
    void begin(long baud);

    /**
     * @brief 更新函數，處理接收到的數據
     */
    void update();

    /**
     * @brief 檢查是否有可用的封包
     * @return 如果有封包可用返回 true，否則返回 false
     */
    bool isPacketAvailable() const;

    /**
     * @brief 獲取一個封包
     * @param packet 用於存儲獲取到的封包
     * @return 如果成功獲取封包返回 true，否則返回 false
     */
    bool getPacket(Packet& packet);

    /**
     * @brief 發送一個封包
     * @param type 封包類型
     * @param data 封包數據
     * @param length 數據長度
     */
    void sendPacket(uint8_t type, const uint8_t* data, uint8_t length);
};

#endif // BLUETOOTH_MANAGER_H