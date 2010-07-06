// AVR architecture globals.

#pragma once

#include "../../globals.hpp"

// Flash storage
#include <avr/pgmspace.h>
// Interrupts
#include <avr/interrupt.h>
// Eeprom
#include <avr/eeprom.h>

// Global AVRLib defines
#include <avrlib/avrlibdefs.h>
// Global AVRLib types
#include <avrlib/avrlibtypes.h>

// Pins (C++ class wrappers)
#include <AVR_Objects/io.hpp>

// Memset
#include <string.h>

// Define PROGMEM and PSTR properly, so as to avoid avr-gcc warnings.
#undef PROGMEM
#define PROGMEM __attribute__(( section(".progmem.data") ))
#undef PSTR
#define PSTR(s) (__extension__({static prog_char __c[] PROGMEM = (s); &__c[0];})) 

extern "C" {
#include <stddef.h>
#include <stdlib.h>
}

#define assert(...)
//inline void assert(){ }

// AVR-specific nop() implementation.
inline void nop(){ asm("sleep"); }

// main() never returns for AVR cores.
void main() __attribute__ ((noreturn));

// Memory block comparison.
inline uint8_t memcmp(const void* s1_void, const void* s2_void, uint8_t n){
  const uint8_t* s1 = (const uint8_t*) s1_void;
  const uint8_t* s2 = (const uint8_t*) s2_void;
  for(; n > 0; n--){
    if(*s1 != *s2)
      return (*s1 > *s2)? 1 : -1;
    s1++;
    s2++;
  }
  return 0;
}

#include "DynamicMemory.hpp"

#include "debug.hpp"

