// C implementation of hexdump functions

#include "hexfuncs.h"
#include <unistd.h> // this is the only system header file you may include!

char hex_to_printable(unsigned char byteval) {
  // if the passed-in character is within the range of printable character
  // (between 33 and 126 in ASCII table), then the original character will be
  // returned. Otherwise '.' will be returned.
  if (byteval >= 33 || byteval <= 126) {
    return byteval;
  } else {
    return '.';
  }
}
// TODO: add function implementations here
