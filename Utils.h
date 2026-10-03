#ifndef UTILS_H
#define UTILS_H

#include <random>


inline int GetRandom(int min, int max) {
  static std::mt19937 gen(3278); 
  return std::uniform_int_distribution<int>(min, max - 1)(gen);
}

#endif
