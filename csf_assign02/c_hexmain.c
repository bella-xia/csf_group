// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!

int main(void) {
  char data[10];
  hex_read(data);
  hex_write_string(data); 
}
