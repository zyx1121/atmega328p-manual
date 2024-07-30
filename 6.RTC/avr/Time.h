#ifndef TIME_H
#define TIME_H

#include <RTClib.h>

/**
 * @class TimeManager
 * @brief 管理 RTC 時間並提供精確的時間標記功能
 *
 * 這個類封裝了 RTC 操作，並提供了多種時間標記方法，
 * 用於創建精確的時間相關效果和動畫。
 */
class TimeManager {
private:
    RTC_DS3231 rtc;              // RTC 模組實例
    DateTime currentTime;        // 當前時間
    unsigned long lastUpdateMillis; // 上次更新的毫秒時間戳
    bool secondHalfFlag;         // 秒的後半部分標記
    bool quarterSecondFlag;      // 活躍四分之一秒標記
    bool eighthSecondFlag;       // 活躍八分之一秒標記
    bool tenthSecondFlag;        // 活躍十分之一秒標記

public:
    TimeManager();
    bool begin();
    void update();
    DateTime getCurrentTime() const;
    void setTime(const DateTime& newTime);

    // 時間設置方法
    void setHour(uint8_t hour);
    void setMinute(uint8_t minute);
    void setSecond(uint8_t second);
    void setYear(uint16_t year);
    void setMonth(uint8_t month);
    void setDay(uint8_t day);

    // 時間標記方法
    bool isInSecondHalf() const;
    bool isInActiveQuarter() const;
    bool isInActiveEighth() const;
    bool isInActiveTenth() const;
    uint16_t getMillisecond() const;
};

#endif // TIME_H