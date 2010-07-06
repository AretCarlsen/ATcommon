// Copyright (C) 2010, Aret N Carlsen (aretcarlsen@autonomoustools.com).
// Array-based data buffer classes (C++).
// Licensed under GPLv3 and later versions. See license.txt or <http://www.gnu.org/licenses/>.


#pragma once

#include "../Status/Status.hpp"
#include "../DataTransfer/DataTransfer.hpp"
#include "../MemoryPool/MemoryPool.hpp"

namespace DataStore {

template <typename Data_t, typename Capacity_t>
class ArrayBuffer { // : public DataTransfer::DataSink<Data_t, Status::Status_t>, public DataTransfer::DataSource<Data_t, Status::Status_t>
//protected:
public:
  Capacity_t capacity;
  Capacity_t size;
  Data_t *buffer;

public:
  ArrayBuffer(Data_t* const new_buffer = NULL, const Capacity_t new_capacity = 0, const Capacity_t new_size = 0)
  : capacity(new_capacity),
    size(new_size),
    buffer(new_buffer)
  { }

  bool is_full() const{
    //DEBUGprint("DAB::is_full: size %d capac %d\n", get_size(), get_capacity());
    return get_size() >= get_capacity();
  }
  bool is_empty() const{
    return get_size() == 0;
  }

  Capacity_t get_availableCapacity() const{
    return (get_capacity() - get_size());
  }

  Data_t get(const uint8_t i = 0) const{
    assert(i < capacity);

  // Get
    return *(buffer + i);
  }

  void set(const Capacity_t i, const Data_t &new_value){
    assert(i < capacity);

  // Set
    *(buffer + i) = new_value;

  // Size increase?
    if(size <= i)
      size = i + 1;
  }

  void append(const Data_t &new_value){
    // DEBUGprint("DAB::append st. buffer=%d, size=%d, capacity=%d\n", (int32_t) buffer, size, capacity);
  // Does not reallocate!
    assert(size < capacity);

    *(buffer + size) = new_value;
    size++;
  }

// Returns a pointer to the first byte in the buffer.
  Data_t* front() const{
    return buffer;
  }
// Returns a pointer to the byte JUST AFTER the last filled byte.
// (NOT after the last byte in the buffer, if the buffer is not full.)
  Data_t* back() const{
    return buffer + size;
  }

  Capacity_t get_size() const{
    return size;
  }
  void set_size(const Capacity_t new_size){
    size = new_size;
  }
  void set_empty(){
    set_size(0);
  }

  Capacity_t get_capacity() const{
    return capacity;
  }

// FIFO push and pop
  Status::Status_t sinkData(const Data_t &new_value){
    // DEBUGprint("DAB::sinkData st\n");
// Check if full
    if(is_full())
      return Status::Status__Bad;
    //DEBUGprint("DAB::sinkData: Not full\n");
    append(new_value);
    return Status::Status__Good;
  }

  Status::Status_t sourceData(Data_t &new_value){
// Check if empty
    if(is_empty())
      return Status::Status__Bad;

    new_value = get();
    return Status::Status__Good;
  }
};

/*
template <typename Data_t, typename Capacity_t>
class StaticArrayBuffer : public ArrayBuffer<Data_t, Capacity_t> {
public:

  StaticArrayBuffer(Data_t *new_buffer = NULL, Capacity_t new_capacity = 0)
  : ArrayBuffer<Data_t, Capacity_t>(new_buffer, new_capacity)
  { }
};
*/

template <typename Data_t, typename Capacity_t>
class DynamicArrayBuffer : public ArrayBuffer<Data_t, Capacity_t> {
private:
  MemoryPool *memoryPool;

public:

  DynamicArrayBuffer(MemoryPool *new_memoryPool, const Capacity_t new_capacity = 0, Data_t* const new_buffer = NULL)
  :  ArrayBuffer<Data_t, Capacity_t>(new_buffer, new_capacity),
     memoryPool(new_memoryPool)
  {
  // Allocate initial buffer, if necessary
    if(new_buffer == NULL && new_capacity > 0){
      ArrayBuffer<Data_t, Capacity_t>::capacity = 0;
      set_capacity(new_capacity);
    }
  }
  ~DynamicArrayBuffer(){
    DEBUGprint_HEAP("DAB: buffer=X%x, mP=X%x\n", (uint32_t) ArrayBuffer<Data_t, Capacity_t>::buffer, (uint32_t) memoryPool);
    if(ArrayBuffer<Data_t, Capacity_t>::buffer != NULL && memoryPool != NULL){
//      DEBUGprint("DAB: free %x\n", ArrayBuffer<Data_t, Capacity_t>::capacity * sizeof(Data_t));
// HEAP
      memoryPool->free(ArrayBuffer<Data_t, Capacity_t>::buffer, ArrayBuffer<Data_t, Capacity_t>::capacity * sizeof(Data_t));
    }
  }

  MemoryPool *get_memoryPool(){
    return memoryPool;
  }

// Reallocate to a new capacity
  bool set_capacity(const Capacity_t new_capacity);

  inline bool set_availableCapacity(const Capacity_t &new_availableCapacity){
    return set_capacity(ArrayBuffer<Data_t, Capacity_t>::get_size() + new_availableCapacity); 
  }

  bool setExpand(const Capacity_t i, const Data_t &data);
  bool sinkExpand(const Data_t &data, const Capacity_t capacity_increment, const Capacity_t capacity_limit);
};

// Reallocate to a new capacity
template <typename Data_t, typename Capacity_t>
bool DynamicArrayBuffer<Data_t, Capacity_t>::set_capacity(const Capacity_t new_capacity){
  //DEBUGprint("DAB::set_cap: %d\n", new_capacity);
  // Trivial case
  if(new_capacity == ArrayBuffer<Data_t, Capacity_t>::capacity)
    return true;

  // Capacity to 0
  if(new_capacity == 0){
    if(ArrayBuffer<Data_t, Capacity_t>::buffer != NULL){
// HEAP
      memoryPool->free(ArrayBuffer<Data_t, Capacity_t>::buffer, ArrayBuffer<Data_t, Capacity_t>::capacity);
      ArrayBuffer<Data_t, Capacity_t>::buffer = NULL;
      ArrayBuffer<Data_t, Capacity_t>::capacity = 0;
    }
    return true;
  }

//  DEBUGprint("DAB: realc %d frm %d ... fld", new_capacity * sizeof(Data_t), ArrayBuffer<Data_t, Capacity_t>::capacity);
  // Attempt to (re)allocate the larger capacity, preserving current contents if applicable.
// HEAP
  Data_t *newBuffer = (Data_t*) memoryPool->realloc(ArrayBuffer<Data_t, Capacity_t>::buffer, ArrayBuffer<Data_t, Capacity_t>::capacity, new_capacity * sizeof(Data_t));

  // Check for allocation failure.
  if(newBuffer == NULL){
    DEBUGprint_HEAP("DAB: realc %d to %d ... fld\n", ArrayBuffer<Data_t, Capacity_t>::capacity, new_capacity * sizeof(Data_t));
//    DEBUGprint("fld\n");
    return false;
  }else{
//    if(new_capacity * sizeof(Data_t) != 20)
//      DEBUGprint("DAB: realc %d frm %d ... scs\n", new_capacity * sizeof(Data_t), ArrayBuffer<Data_t, Capacity_t>::capacity);
  }
  // Save new location.
  ArrayBuffer<Data_t, Capacity_t>::buffer = newBuffer;
  // Save new capacity; trim size if necessary
  ArrayBuffer<Data_t, Capacity_t>::capacity = new_capacity;
  if(ArrayBuffer<Data_t, Capacity_t>::size > ArrayBuffer<Data_t, Capacity_t>::capacity)
    ArrayBuffer<Data_t, Capacity_t>::size = ArrayBuffer<Data_t, Capacity_t>::capacity;

  // Success.
  return true;
}

template <typename Data_t, typename Capacity_t>
bool DynamicArrayBuffer<Data_t, Capacity_t>::setExpand(const Capacity_t i, const Data_t &data){
// Expand, if necessary
  if(i >= ArrayBuffer<Data_t, Capacity_t>::get_capacity() && !set_capacity(i))
    return false;

  set(i, data);

  return true;
}

// Append a data byte to a packet, expanding the packet's capacity if necessary.
// Will not expand beyond capacity_limit.
template <typename Data_t, typename Capacity_t>
bool DynamicArrayBuffer<Data_t, Capacity_t>::sinkExpand(const Data_t &data, const Capacity_t capacity_increment, const Capacity_t capacity_limit){
// Attempt to sink
  if(sinkData(data) != Status::Status__Good){
  // Attempt to expand and sink again, or fail. (Don't expand beyond capacity_limit.)
    if((ArrayBuffer<Data_t, Capacity_t>::get_capacity() + capacity_increment > capacity_limit)
       || !(
               set_capacity(ArrayBuffer<Data_t, Capacity_t>::get_capacity() + capacity_increment)
            && (sinkData(data) == Status::Status__Good)
       )
    )
      return false;
  }

  return true;
}


// End namespace: DataStore
}

