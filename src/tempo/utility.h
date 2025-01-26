#include "WString.h"
#pragma once

#include "HardwareSerial.h"
#include <math.h>

struct FixedSize {
  String data;
  int size;

  template<typename T>
  FixedSize::FixedSize(T data);

  template<typename T>
  FixedSize::FixedSize(T data, int size);
  
};

namespace PrinterImpl {

template<typename T>
String to_string(T data) {
  return String(data);
}

template<>
String to_string<bool>(bool data) {
  return data ? "true" : "false";
}

template<>
String to_string<FixedSize>(FixedSize data) {
  String s;
  for (auto i = data.data.length(); i < data.size; ++i)
    s += ' ';
  s += data.data;
  return s;
}


template<typename T>
void print_impl(T data, int size = -1) {
  Serial.print(data);
}



template<>
void print_impl(int data, int size = -1) {
  if (size != -1) {
    auto s = size - log10(data);
    for (auto i = 0; i < s; i++)
      Serial.print(" ");
  }
  Serial.print(data);
}

template<>
void print_impl(bool f, int size = -1) {
  Serial.print(f ? "true ---" : "false -- ");
}

}

template<typename T>
FixedSize::FixedSize(T data) {
  data = PrinterImpl::to_string(data);
}
template<typename T>
FixedSize::FixedSize(T data, int size)
  : size{ size } {
  this->data = PrinterImpl::to_string(data);
}


template<class T, class U>
struct is_same {
  static constexpr bool value = false;
};

template<class T>
struct is_same<T, T> {
  static constexpr bool value = true;
};

namespace Utility {

template<typename T>
void bound(const T& lbound, T& var, const T& ubound) {
  if (var < lbound)
    var = lbound;
  if (var > ubound)
    var = ubound;
}

template<typename T>
T map(T x, T in_min, T in_max, T out_min, T out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}




template<typename First, typename... Types>
void print_impl(First f, Types... others) {

  auto s = PrinterImpl::to_string(f);

  // String s;
  // if (is_same<First, bool>::value)
  //   s = f ? "true 3" : "false 4";
  // else
  //   s = String(f);

  // PrinterImpl::print_impl(f, 10);
  Serial.print(s);
  if constexpr (sizeof...(Types))
    print_impl(others...);
  else
    Serial.println();
}

template<typename... Types>
void print_impl(bool f, Types... others) {
  Serial.print(f ? "true ** " : "false **");
  if constexpr (sizeof...(Types))
    print_impl(others...);
  else
    Serial.println();
}

template<typename... Types>
void print(const Types&... args) {
  print_impl(args...);
}


void fatal(const char* msg) {
  Serial.print("FATAL: ");
  Serial.println(msg);
  while (1) {}
}

};