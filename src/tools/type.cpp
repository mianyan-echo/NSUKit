//
// Created by 56585 on 2023/8/16.
//
#include "type.h"


/**
 * 重载 “|” 操作符
 * @param lhs 左值
 * @param rhs 右值
 * @return 按位与后的枚举值
 */
nsukitStatus_t operator |(nsukitStatus_t lhs, nsukitStatus_t rhs) {
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
