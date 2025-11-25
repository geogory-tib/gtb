#include "gtd.h"
#include <cstdlib>
#include <cstdio>


const char  gooner[] = "Hello gooner";

int main() {
  auto chbuffer = Dyn_Arry_New<char>(sizeof(gooner));
  chbuffer.append_arr((char *)gooner, sizeof(gooner));
  std::puts(chbuffer.buffer);
  auto e = chbuffer[15];
  putchar(e);
  return 0;
}
