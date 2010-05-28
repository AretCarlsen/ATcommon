// Copyright (C) 2010, Aret N Carlsen (aretcarlsen@autonomoustools.com).
// Ringbuffer (C++).
// Licensed under GPLv3 and later versions. See license.txt or <http://www.gnu.org/licenses/>.


#pragma once

#include <stdint.h>

#include "../Status/Status.hpp"
#include "Buffer.hpp"

namespace DataStore {

// Generic variable ringbuffer.
// FIFO push/pop is thread-safe as long as only one thread is pushing and one thread is popping simultaneously.
// Note that there is no buffer size limit, except for the innate machine memory size limits.
//
// Where applicable, returns 0 for success, 1 for failure.
template <class Data_t, typename Capacity_t>
class RingBuffer { // : public DataTransfer::DataSink<Data_t, Status::Status_t>, public DataTransfer::DataSource<Data_t, Status::Status_t>
private:
// Buffer pointers
  Data_t *bufferStart;
  Data_t *bufferEnd;
// Write pointers
  Data_t *writePtr;
  volatile Capacity_t writeCount;
// Read pointers
  Data_t *readPtr;
  volatile Capacity_t readCount;

public:
// Static, preallocated buffer and capacity
  RingBuffer(Data_t *raw_buffer, Capacity_t capacity){
    setBuffer(raw_buffer, capacity);
  }

// Set internal buffer
  void setBuffer(Data_t *raw_buffer, Capacity_t capacity){
    bufferStart = raw_buffer;
    bufferEnd = bufferStart + capacity - 1;
    reset();
  }

// Reset buffer. Buffer location / capacity is not affected.
// NOT thread-safe!
  void reset(){
    readCount = 0;
    writeCount = 0;
    writePtr = bufferStart;
    readPtr = bufferStart;
  }

// Empty out the buffer. Thread-safe, if called by reader.
  void empty(){
    while(! is_empty()) get();
  }

  bool is_full(){
    return (get_size() >= get_capacity());
  }
  bool is_empty(){
    // return (get_size() == 0);
    return (readCount == writeCount);
  }

  Capacity_t get_availableCapacity(){
    return (get_capacity() - get_size());
  }
 
// Get buffer size (used capacity)
  Capacity_t get_size(){
    return (writeCount - readCount);
  }
 
// Get total buffer capacity 
  Capacity_t get_capacity(){
    return (bufferEnd - bufferStart + 1);
  }
 
// Append (FIFO-push) data.
  // Uses '=' operator. Class should be initialized as Buffer<class*>
  // if '=' is not available.
  void append(const Data_t data){
    assert(! is_full());

    // Store data  
    *writePtr = data;

    // Increment stored data tracker
    increment_write_position();
  }
// Get (FIFO-pop) data 
  Data_t get(){
    assert(! is_empty());
    
    // Extract data
    Data_t *data = readPtr;

    increment_read_position();

    // Return actual data.
    return *data;
  }
  Data_t get_in_place(){
    assert(! is_empty());
    
    // Extract data
    return *readPtr;
  }
  void increment_read_position(){
    assert(! is_empty());

  // If the read pointer is at the end of the buffer, loop back to beginning.
    if(readPtr == bufferEnd)
      readPtr = bufferStart;
  // Simple increment
    else
      readPtr++;

    readCount++;
  }
  void increment_write_position(){
    assert(! is_full());

  // If the write pointer is at the end of the buffer, loop back to beginning.
    if(writePtr == bufferEnd)
      writePtr = bufferStart;
  // Simple increment
    else
      writePtr++;

    writeCount++;
  }

// FIFO push and pop

  Status::Status_t sinkData(Data_t new_value){
// Check if full
    if(is_full())
      return Status::Status__Bad;

    append(new_value);
    return Status::Status__Good;
  }

  Status::Status_t sourceData(Data_t *new_value){
// Check if empty
    if(is_empty())
      return Status::Status__Bad;

    *new_value = get();
    return Status::Status__Good;
  }
};

// End namespace: DataStore
}

