#ifndef ARENA_H
#define ARENA_H
#include <stddef.h>
#include <stdlib.h>
#define ARENA_FULL -1
/*
  struct for the arena brk is the "break"
  when you push onto it your given the break of the arena
  and the brk is moved up by the size of your allocation
*/
typedef struct arena_t {
  void *buffer;
  int brk;
  int cap;
} arena;
static inline arena arena_new(int cap) {
  arena ret;
  ret.brk = 0;
  ret.cap = cap;
  ret.buffer = calloc(cap, 1);
  return ret;
}
/*returns NULL if arena is full*/
static inline void *arena_alloc(arena *ar, int size) {
  if ((ar->brk + size - 1) >= ar->cap) {
    return NULL;
  }
  void *ret = (((char *)ar->buffer) + ar->brk);
  ar->brk += (size);
  return ret;
}
/*returns -1 if full*/
static inline int arena_used(arena *ar) {
  if (ar->brk >= ar->cap) {
    return ARENA_FULL;
  }
  return ar->brk;
}
/*frees the current arena*/
static inline void arena_free(arena *ar) {
  free(ar->buffer);
  ar->buffer = NULL;
  ar->cap = 0;
  ar->brk = 0;
}
/*
  Holds a dynamic array of buffers that contains seperate "pages"(basically arenas)
  the page size is determined when you use the garena_new function.
*/
typedef struct garena_t {
  arena *pages;
  int page_size;
  int current_page;
  int page_count;
} garena;

static inline garena garena_new(int page_size) {
  garena ret;
  ret.page_size = page_size;
  ret.current_page = 0;
  ret.page_count = 10;
  ret.pages = (arena *)calloc(ret.page_count,sizeof(arena)); 
  ret.pages[ret.current_page] = arena_new(ret.page_size);
  return ret;
}
/*
  internal function there really isn't a usecase for this unless
  you want to allocate your own pages.
*/
static inline arena *garena_page_new(garena *ar) {
  if (ar->current_page + 1 >= ar->page_count) { 
    ar->page_count += 10;
    ar->pages = (arena *)realloc(ar->pages, (sizeof(arena) * ar->page_count));
  }
  ar->pages[ar->current_page + 1] = arena_new(ar->page_size);
  ar->current_page++;
  arena *current_page = &ar->pages[ar->current_page];
  return current_page;
}
/*can return NULL on failure*/
static inline void *garena_alloc(garena *ar, int size) {
  arena *current_page = &ar->pages[ar->current_page];
  if ((current_page->brk + size) >= current_page->cap) {
    current_page = garena_page_new(ar);
  }
  void *ret = arena_alloc(current_page, size);
  return ret;
}
/*returns the amount used of the current page return -1 if current page is full*/
static inline int garena_used(garena *ar) {
  arena *current_page = &ar->pages[ar->current_page];
  if (current_page->brk >= current_page->cap) {
    return ARENA_FULL;
  }
  return current_page->brk;
}
/*deconstructor for the garena*/
static inline void garena_destroy(garena *gar) {
  for (int i = gar->current_page; i >= 0; i--) {
    //arena *current_page = &gar->pages[i];
    arena_free( &gar->pages[i]);
  }
  gar->page_size = 0;
  gar->current_page = 0;
  gar->page_count = 0;
  free(gar->pages);
}

#endif // !
