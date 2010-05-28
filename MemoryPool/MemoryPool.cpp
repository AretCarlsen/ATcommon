#include "MemoryPool.hpp"

void* MemoryPool::realloc(void* old_ptr, Capacity_t old_size, Capacity_t new_size){
// Request an allocation.
  DEBUGprint_HEAP("MP: ralc ptr X%x frm %d to %d, cur alc %d\n", (uint32_t) old_ptr, old_size, new_size, allocated);

  // See if pool has enough to spare
  if(! allocate(new_size))
    return NULL;

  // Try to realloc
  void* new_ptr = ::realloc(old_ptr, new_size);

  // Deallocate if realloc failed.
  if(new_ptr == NULL){
    DEBUGprint_HEAP("MP: ralc fld\n");
    deallocate(new_size);
  }

  deallocate(old_size);
  DEBUGprint_HEAP("MP: ralc scs, ptr X%x, ttl alc %d\n", (uint32_t) new_ptr, allocated);

  // Return new (or NULL)
  return new_ptr;
}

// Free/Deallocate
void MemoryPool::free(void* ptr, Capacity_t size){
  DEBUGprint_HEAP("MP: free ptr X%x sz %d\n", (uint32_t) ptr, size);
  if(ptr == NULL){
    DEBUGprint_HEAP("MP: free(NULL)!\n");
    return;
  }

  // Free memory
  ::free(ptr);
  // Add memory size to pool tracker
  deallocate(size);

  DEBUGprint_HEAP("MP: aft free, ttl alc %d\n", allocated);
}

// Allocate
bool MemoryPool::allocate(Capacity_t size){
  DEBUGprint_HEAP("MP: raw-alc %d: ", size);
  // Memory available in pool?
  if(size > get_availableAllocation() || (
    parentPool != NULL && !(parentPool->allocate(size))
  )){
    DEBUGprint_HEAP("fld\n");
    return false;
  }
  DEBUGprint_HEAP("scs\n");

  // Track allocated memory.
  allocated += size;
  return true;
}
// Deallocate

void MemoryPool::deallocate(Capacity_t size){
  DEBUGprint_HEAP("MP: raw-dealc %d\n", size);
  // Don't underflow.
  if(allocated < size){
    DEBUGprint_HEAP("MP: raw-dealc > allcd!\n");
    allocated = 0;
  }else
    allocated -= size;

  if(parentPool != NULL)
    parentPool->deallocate(size);

  DEBUGprint_HEAP("MP: aft rdalc, ttl alc %d\n", allocated);
}

