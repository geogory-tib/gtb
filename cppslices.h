#ifndef CPPSLICES_H
extern "C"{
#include <stddef.h>
#include <stdlib.h>
}
#define SLICE_FREE(slice) \
  free((slice).buffer);\
(slice).len = 0;
//alloc_params is able to be used so you can use custom allocators with this macro 
//the size is what is assigned to the length of the slice

template<typename T>
  struct cslice{
    size_t len;
    T *buffer;
    inline cslice<T> slice(size_t bI, size_t eI){
      cslice<T>ret;
      ret.buffer = &buffer[bI];
      ret.len = eI - bI;
      return ret;
    }
    T *operator[](size_t index){
      return &buffer[index];
    }
    
  };

#endif // !CPPSLICES_H
