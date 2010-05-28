
#pragma once

#ifndef DEBUGprint_HEAP
#define DEBUGprint_HEAP(...)
#endif

class MemoryPool {
  MemoryPool *parentPool;

// Could be moved into a template declaration.
  typedef uint16_t Capacity_t;

// Current allocated size; allocation size limit.
  Capacity_t allocated;
  Capacity_t maxAllocated;

public:
  MemoryPool(Capacity_t new_maxAllocated)
  : parentPool(NULL), allocated(0), maxAllocated(new_maxAllocated)
  { }

  inline Capacity_t get_availableAllocation(){
  // Does not take into account parent limits
    return (maxAllocated - allocated);
  }

// Alias for realloc(null, size, 0).
  inline void* malloc(Capacity_t size){
    return realloc(NULL, 0, size);
  }
  void* realloc(void* old_ptr, Capacity_t old_size, Capacity_t new_size);

  void free(void* ptr, Capacity_t size);

  bool allocate(Capacity_t size);
  void deallocate(Capacity_t size);
};


/*
template <typename Class_t>
class ClassMemoryPool {
  void* new
};
*/

/*
class ChildMemoryPool {
// Alias for realloc(null, size, 0).
  inline void* malloc(Capacity_t size){
    return realloc(NULL, 0, size);
  }

  void* realloc(void* old_ptr, Capacity_t old_size, Capacity_t new_size);

  void free(void* ptr, Capacity_t size);

  bool allocate(Capacity_t size);
  void deallocate(Capacity_t size);
};
*/

