// C implementation of hexdump functions

#include "hexfuncs.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // this is the only system header file you may include!

char hex_to_printable(unsigned char byteval) {
  // if the passed-in character is within the range of printable character
  // (between 33 and 126 in ASCII table), then the original character will be
  // returned. Otherwise '.' will be returned.
  if (byteval >= 33 && byteval <= 126) {
    return byteval;
  } else {
    return '.';
  }
}

void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]) {
  *sbuf = (byteval / 16 >= 10) ? (byteval / 16 + 'A') : (byteval / 16 + '0');
  *(sbuf + 1) =
      (byteval % 16 >= 10) ? (byteval % 16 + 'A') : (byteval % 16 + '0');
  printf("char1:  %c\n", *(sbuf));
  printf("char2:  %c\n", *(sbuf + 1));
}

void hex_format_offset(unsigned offset, char sbuf[]) {
  int index = 0;
  printf("current offset is : %ud\n", offset);
  for (; index < 4; index++) {
    unsigned byteval = hex_to_printable(offset % 256);
    printf("current int is : %ud\n", byteval);
    printf("String is : %s\n", sbuf);
    hex_format_byte_as_hex(byteval, (sbuf + 6 - index * 2));
    offset = offset >> 8;
    printf("current offset is : %ud\n", offset);
  }
}

// TODO: add function implementations here
