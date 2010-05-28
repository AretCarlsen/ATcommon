// Copyright (C) 2010, Aret N Carlsen (aretcarlsen@autonomoustools.com).
// Fixed-point fraction handling (C++).
// Licensed under GPLv3 and later versions. See license.txt or <http://www.gnu.org/licenses/>.


// Fixed-point library: 32-bit with 8-bit fractional precision

#pragma once

#include "../globals.hpp"


// Native storage type, number of significand bits, and intermediate calculation type.
template <typename Native_t, uint8_t SignificandBits, typename Intermediate_t = int32_t>
class FixedPoint{
private:
  Native_t value;

  static const Native_t minValue = __MIN_SIGNED(Native_t);
  static const Native_t maxValue = __MAX_SIGNED(Native_t);

public:

  static const uint8_t SignificandBitCount = SignificandBits;

// Create a new FixedPoint object from a given integer or fixedPoint Native_t value.
  FixedPoint(Native_t initialValue = 0, bool isNative = false){
    if(isNative)
      value = initialValue;
    else
      fromInt32(initialValue); 
  }
/*
// Promotion = unpleasant.
  FixedPoint(int32_t initialValue, bool isNative = false){
    if(isNative)
      value = initialValue;
    else
      fromInt32(initialValue); 
  }
*/

  inline int toInt() const{
    return getInteger();
  }

  // Convert fixed-point number to integer. (With rounding, by default.)
  inline int getInteger(bool round = true) const{
    if(   round
       && ( value & (1 << (SignificandBits - 1)) )
      ){
      // bit behind the point was a '1'
      // round up to next higher integer
      return (value >> SignificandBits) + 1;
    }else{
      // bit behind the point was a '0'
      // round down (truncate) to next lower integer
      return (value >> SignificandBits);
    }
  }

  inline unsigned int getSignificand() const{
  // This might not work if entire value is significand.
    return value & ((1 << SignificandBits) - 1);
  }    

  inline void fromInt32(const int32_t &new_int_value){
    value = int32ToNative(new_int_value);
  }
  inline Native_t int32ToNative(const int32_t &new_int_value) const{
    return new_int_value << SignificandBits;
  }

  inline void set_rawValue(const Native_t &new_value){
    value = new_value;
  }
  inline Native_t get_rawValue() const{
    return value;
  }

// OPERATORS

/* ASSIGNMENT */
  inline void operator=(const int32_t &new_int_value){
    fromInt32(new_int_value);
  }
  inline void operator=(const FixedPoint<Native_t, SignificandBits> &new_FP_value){
    value = new_FP_value.value;
  }

/* UNARY */
  inline FixedPoint operator-() const{
  // Check for negative that cannot be represented as positive
    if(value == minValue)
      return FixedPoint(maxValue, true);
    return FixedPoint(-value, true);
  }

/* ADDITION / SUBTRACTION */
private:
  inline void internal_add(const Native_t &b_value){
    setValueOverflow_Intermediate((int32_t) value + b_value);
  }
public:
  inline FixedPoint operator+(const FixedPoint<Native_t, SignificandBits> &b) const{
  // add a and b (a+b) with fixed-point math
    FixedPoint new_value(value, true);
    new_value += b;
    return new_value;
  }
  inline void operator+=(const FixedPoint<Native_t, SignificandBits> &b){
  // Add a and b (a+b).
    internal_add(b.value);
  }
  inline FixedPoint operator-(const FixedPoint<Native_t, SignificandBits>& b) const{
  // subtract a and b (a-b) with fixed-point math
    FixedPoint new_value(value, true);
    new_value -= b;
    return new_value;
  }
  inline void operator-=(const FixedPoint<Native_t, SignificandBits>& b){
  // subtract a and b (a-b) with fixed-point math
  // Translate to addition of a negative.
    internal_add(-(b.value));
  }

/* MULTIPLICATION / DIVISION */
private:
  inline void setValueOverflow_Intermediate(Intermediate_t new_value){
  // Set the native value, checking for overflow.
    if(new_value > maxValue) value = maxValue;
    else if(new_value < minValue) value = minValue;
    else value = new_value;
  }

public:
  inline FixedPoint operator*(const FixedPoint<Native_t, SignificandBits>& b) const{
// Multiply a and b (a*b) with fixed-point math.
  // Computes in intermediate type.
    FixedPoint new_value(value, true);
    new_value *= b;
    return new_value;
  }
  inline void operator*=(const FixedPoint<Native_t, SignificandBits>& b){
// Multiply a and b (a*b).
  // Computes in intermediate type, as a reducing bit-shift occurs *after* the multiplication.
  // Check for overflow, as result may occupy up to double the maxValue.
    setValueOverflow_Intermediate(((Intermediate_t) value * b.value) >> SignificandBits);
  }
  inline FixedPoint operator/(const FixedPoint<Native_t, SignificandBits>& denominator) const{
// Divide the numerator (this) by a denominator.
    FixedPoint new_value(value, true);
    new_value /= denominator;
    return new_value;
  }
  inline void operator/=(const FixedPoint<Native_t, SignificandBits>& denominator){
// Divide the numerator (this) by a denominator.
  // Check for div-by-0.
    if(denominator.value == 0){
    // 0/0 = 0
      if(value == 0) return;
    // +/0 = +maxValue; -/0 = -maxValue
      value = (value > 0)? maxValue : minValue;
      return;
    }
  // Computes in intermediate type, as an expanding bit-shift occurs *before* the division.
  // Check for overflow (since denominator may have been less than 0).
    setValueOverflow_Intermediate(((Intermediate_t) value << SignificandBits) / denominator.value);
    
  }
/* COMPARISON */
  inline bool  operator<(const FixedPoint<Native_t, SignificandBits> &b) const{
    return (value < b.value);
  }
  inline bool operator<=(const FixedPoint<Native_t, SignificandBits> &b) const{
    return (value <= b.value);
  } 
  inline bool operator==(const FixedPoint<Native_t, SignificandBits> &b) const{
    return (value == b.value);
  }
  inline bool operator!=(const FixedPoint<Native_t, SignificandBits> &b) const{
    return (value != b.value);
  }
  inline bool operator>=(const FixedPoint<Native_t, SignificandBits> &b) const{
    return (value >= b.value);
  }
  inline bool  operator>(const FixedPoint<Native_t, SignificandBits> &b) const{
    return (value > b.value);
  }
};

