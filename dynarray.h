#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H
extern "C" {
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
}
#define ALLOC_FAILURE -1
template <typename T> struct Dyn_Arry {
  size_t cap;
  size_t len;
  T *buffer;
  /*pushes elements to the back will allocate if cap is too small*/
  int append(T element) {
    if (len >= cap) {
      int pass = grow(10);
      if (pass == ALLOC_FAILURE) {
        return ALLOC_FAILURE;
      }
    }
    buffer[len] = element;
    len++;
    return 0;
  }
  int append_arr(T *ptr, size_t size) {
    if (len + size > cap) {
      int code = grow((cap + size) + 10);
      if (code == ALLOC_FAILURE) {
        return ALLOC_FAILURE;
      }
    }
    for (int i = 0; i < size; i++) {
      buffer[len] = ptr[i];
      len++;
    }
    return 0;
  }
  /* reallocs the buffer and updates the meta data of the struct*/
  int grow(size_t size) {
    T *tmp = (T *)realloc(buffer, (cap + size) * sizeof(T));
    if (tmp == NULL) {
      return ALLOC_FAILURE;
    }
    buffer = tmp;
    cap += size;
    return 0;
  }
  void free_arr() {
    free(buffer);
    cap = 0;
    len = 0;
  }
  /*zeros the buffer*/
  inline void erase() {
    memset(buffer, 0, cap - 1);
    len = 0;
  }
  /*replaces the element at the given index with the supplied value*/
  inline void replace_at(size_t index, T elem) {
    if (index >= len) {
      printf("Attemp of out of bounds replace. Array with Len:%ld indexed with "
             "%ld\n",
             len, index);
      abort();
    }
    buffer[index] = elem;
  }
  /*This can allocate default is to allocate space for 10 more elments*/
  int insert_at(size_t index, T elem) {
    if (len + 1 >= cap) {
      int suc = grow(10);
      if (suc == ALLOC_FAILURE) {
        return suc;
      }
    }
    len++;
    T holder1 = buffer[index];
    T holder2;
    buffer[index] = elem;
    for (int i = index + 1; i < len + 1; i++) {
      holder2 = buffer[i];
      buffer[i] = holder1;
      holder1 = holder2;
    }
    return 0;
  }
  /*
    This does not rezise the buffer but deletes whatever element and shifts them
    down
  */
  void delete_at(size_t index) {
    T holder1 = buffer[index + 1];
    buffer[index] = holder1;
    for (int i = index + 1; i < len + 1; i++) {
      holder1 = buffer[i];
      buffer[i - 1] = holder1;
    }
    len--;
  }
  // this does not shrink the array just 0s the last element and decrements the
  // len
  inline void pop() {
    buffer[len - 1] = 0;
    len--;
  }
  /*shrinks the array by the given argument */
  int shrink(size_t decrement) {
    T *tmp = (T *)realloc(buffer, (cap - decrement * sizeof(T)));
    if (tmp == NULL) {
      return ALLOC_FAILURE;
    }
    buffer = tmp;
    cap -= decrement;
    return 0;
  }

  T operator[](size_t index) {
    if (index >= len) {
      printf("Out of Bounds Indexing. Array with Len:%ld indexed with %ld\n",
             len, index);
      abort();
    }
    return buffer[index];
  }
};

template <typename T> Dyn_Arry<T> new_dynarray(size_t size) {
  Dyn_Arry<T> ret;
  ret.buffer = (T *)calloc(size, sizeof(T));
  ret.cap = size;
  ret.len = 0;
  return ret;
}

#endif
