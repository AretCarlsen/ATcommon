#pragma once

/* DYNAMIC MEMORY   (from AVRFreaks forum) */
  // Standard new.
  inline void* operator new(size_t n){ return malloc(n); }
  // Array new.
  inline void* operator new[](size_t n){ return malloc(n); }
  // Placement new for allocating the object inside a given memory buffer.
  inline void* operator new(size_t, void* const buf){ return buf; }
  // Standard delete.
  inline void operator delete(void* p){ free(p); }
  // Array delete.
  inline void operator delete[](void* p){ free(p); }
  /// Placement delete called automatically on "placement new" failure.
  inline void operator delete(void* p, void* const buf){ }

