#ifndef UTILS_H
#define UTILS_H

#include <random>

// 回傳 [min, max) 的隨機整數
inline int GetRandom(int min, int max) {
  static std::mt19937 gen(3278);  // 固定 seed，結果可重現
  return std::uniform_int_distribution<int>(min, max - 1)(gen);
}

#endif
