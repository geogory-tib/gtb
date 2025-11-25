#include "gtd.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>


const char  gooner[] = "Hello gooner";

int main() {
  cslice<char> geo;
  SLICE_ALLOC_MALLOC(geo,10,sizeof(char));
  memcpy(geo.buffer, gooner, sizeof(gooner));
  puts(geo.buffer);
  panic("goooning");
  return 0;
}
