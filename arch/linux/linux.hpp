#pragma once

#include "../../globals.hpp"

//#include <iostream>
//#include "./common/FixedPoint/FixedPoint.hpp"
//std::ostream& operator<<(std::ostream& out, const FixedPoint& a);

// For assert()
#include <assert.h>
// For uint8_t etc
#include <stdint.h>
// For memcpy
#include <string.h>

// Realloc
#include <stdlib.h>

// For placement new
#include <new>

#define EEMEM

inline void eeprom_read_block(void* ram_ptr, void* eeprom_pointer, uint16_t count){
  memcpy(ram_ptr, eeprom_pointer, count);
}
inline uint8_t eeprom_read_byte(uint8_t* eeprom_pointer){
// No sanity checking!
  return *eeprom_pointer;
}

inline void eeprom_write_block(void* ram_ptr, void* eeprom_pointer, uint16_t count){
  memcpy(eeprom_pointer, ram_ptr, count);
}
inline void eeprom_write_byte(uint8_t *eeprom_pointer, uint8_t value){
// No sanity checking!
  *eeprom_pointer = value;
}

#include "debug.hpp"

