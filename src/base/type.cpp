//
// Created by 56585 on 2023/8/16.
//
#include "base/type.h"


bool ThreadSafeEvent::isSet() {
    std::lock_guard<std::mutex> lock(mtx);
    return eventOccurred;
}


void ThreadSafeEvent::setEvent() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        eventOccurred = true;
    }
    cv.notify_one();
}


bool ThreadSafeEvent::waitForEvent(int timeout) {
    std::unique_lock<std::mutex> lock(mtx);
    if (timeout>0) {
        return cv.wait_for(lock, std::chrono::seconds(timeout), [&]{ return eventOccurred; });
    } else {
        cv.wait(lock, [&]{ return eventOccurred; });
    }
    return true;
}


void ThreadSafeEvent::resetEvent() {
    std::lock_guard<std::mutex> lock(mtx);
    eventOccurred = false;
}


/**
 * 重载 “|” 操作符
 * @param lhs 左值
 * @param rhs 右值
 * @return 按位与后的枚举值
 */
nsukitStatus_t operator| (nsukitStatus_t lhs, nsukitStatus_t rhs) {
    return static_cast<nsukitStatus_t>((int)lhs | (int)rhs);
}


/**
 * 重载 “|=” 操作符
 * @param lhs 左值
 * @param rhs 右值
 */
void operator|= (nsukitStatus_t &lhs, nsukitStatus_t rhs) {
    // 传入左值采用传引用方式，而后将这个引用指向一个新的值，并且不用返回新的值
    lhs = static_cast<nsukitStatus_t>((int)lhs | (int)rhs);
}
