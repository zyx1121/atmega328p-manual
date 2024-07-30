#include "time.h"

TimeManager::TimeManager() : lastUpdateMillis(0), secondHalfFlag(false), quarterSecondFlag(false), eighthSecondFlag(false), tenthSecondFlag(false) {}

/**
 * @brief 初始化 RTC 模組
 * @return 如果初始化成功返回 true，否則返回 false
 */
bool TimeManager::begin() {
    if (!rtc.begin()) {
        return false;
    }
    if (rtc.lostPower()) {
        // RTC 失去電源時，使用編譯時間重置
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    return true;
}

/**
 * @brief 更新當前時間和時間標記
 *
 * 這個方法應該在主循環中頻繁調用，以確保時間標記的準確性
 */
void TimeManager::update() {
    currentTime = rtc.now();
    unsigned long currentMillis = millis();
    uint16_t millisInSecond = currentMillis % 1000;

    // 更新各種時間標記
    secondHalfFlag = millisInSecond >= 500;
    quarterSecondFlag = (millisInSecond % 500) >= 250;
    eighthSecondFlag = (millisInSecond % 250) >= 125;
    tenthSecondFlag = (millisInSecond % 200) >= 100;

    lastUpdateMillis = currentMillis;
}

/**
 * @brief 獲取當前時間
 * @return 當前的 DateTime 對象
 */
DateTime TimeManager::getCurrentTime() const {
    return currentTime;
}

/**
 * @brief 設置新的時間
 * @param newTime 要設置的新時間
 */
void TimeManager::setTime(const DateTime& newTime) {
    rtc.adjust(newTime);
    currentTime = newTime;
}

/**
 * @brief 設置小時
 * @param hour 要設置的小時值 (0-23)
 */
void TimeManager::setHour(uint8_t hour) {
    if (hour > 23) hour = 23;
    setTime(DateTime(currentTime.year(), currentTime.month(), currentTime.day(),
                     hour, currentTime.minute(), currentTime.second()));
}

/**
 * @brief 設置分鐘
 * @param minute 要設置的分鐘值 (0-59)
 */
void TimeManager::setMinute(uint8_t minute) {
    if (minute > 59) minute = 59;
    setTime(DateTime(currentTime.year(), currentTime.month(), currentTime.day(),
                     currentTime.hour(), minute, currentTime.second()));
}

/**
 * @brief 設置秒數
 * @param second 要設置的秒數值 (0-59)
 */
void TimeManager::setSecond(uint8_t second) {
    if (second > 59) second = 59;
    setTime(DateTime(currentTime.year(), currentTime.month(), currentTime.day(),
                     currentTime.hour(), currentTime.minute(), second));
}

/**
 * @brief 設置年份
 * @param year 要設置的年份值 (2000-2099)
 */
void TimeManager::setYear(uint16_t year) {
    if (year < 2000) year = 2000;
    if (year > 2099) year = 2099;
    setTime(DateTime(year, currentTime.month(), currentTime.day(),
                     currentTime.hour(), currentTime.minute(), currentTime.second()));
}

/**
 * @brief 設置月份
 * @param month 要設置的月份值 (1-12)
 */
void TimeManager::setMonth(uint8_t month) {
    if (month < 1) month = 1;
    if (month > 12) month = 12;
    setTime(DateTime(currentTime.year(), month, currentTime.day(),
                     currentTime.hour(), currentTime.minute(), currentTime.second()));
}

/**
 * @brief 設置日期
 * @param day 要設置的日期值 (1-31)
 */
void TimeManager::setDay(uint8_t day) {
    if (day < 1) day = 1;
    if (day > 31) day = 31;
    // 注意：這裡沒有檢查每月的實際天數
    setTime(DateTime(currentTime.year(), currentTime.month(), day,
                     currentTime.hour(), currentTime.minute(), currentTime.second()));
}

/**
 * @brief 檢查是否在秒的後半部分
 * @return 如果在秒的後半部分（500ms-999ms）返回 true，否則返回 false
 */
bool TimeManager::isInSecondHalf() const {
    return secondHalfFlag;
}

/**
 * @brief 檢查是否在活躍的四分之一秒內
 * @return 如果在第二個（250ms-499ms）或第四個（750ms-999ms）四分之一秒內返回 true，否則返回 false
 */
bool TimeManager::isInActiveQuarter() const {
    return quarterSecondFlag;
}

/**
 * @brief 檢查是否在活躍的八分之一秒內
 * @return 每 0.125 秒的後半部分返回 true，前半部分返回 false
 */
bool TimeManager::isInActiveEighth() const {
    return eighthSecondFlag;
}

/**
 * @brief 檢查是否在活躍的十分之一秒內
 * @return 每 0.1 秒的後半部分返回 true，前半部分返回 false
 */
bool TimeManager::isInActiveTenth() const {
    return tenthSecondFlag;
}

/**
 * @brief 獲取當前毫秒數
 * @return 返回當前毫秒數（0-999）
 */
uint16_t TimeManager::getMillisecond() const {
    return millis() % 1000;
}