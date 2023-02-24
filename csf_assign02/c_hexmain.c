// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!
int main(void) {
  char data[17] = {' '};
  *(data + 16) = '\0';
  unsigned  placeCount = 0;
  char place[9] = {'0'};
  *(place + 8) = '\0';
  hex_format_offset(placeCount, place); 
  int numDataRead = hex_read(data);
  while(numDataRead != 0){
    //printing the column 0
    hex_write_string(place); // print out the place
    hex_write_string(": "); // get a space between column 0 and column 1
    //printing the column 1
    char tempStore[3];
    tempStore[2] = '\0';
    for(int i = 0; i < numDataRead; i++){
      unsigned char cur = data[i];
      hex_format_byte_as_hex(cur, tempStore);
      hex_write_string(tempStore);
      hex_write_string(" "); // add space between the printed character
    }
    
    for(int i = numDataRead; i < 16; i++){
      hex_write_string("   ");
    }
    hex_write_string(" ");
    //printing the column 2
    char dataPrint[17] = {' '};
    for(int i = 0; i < numDataRead; i++) {
      dataPrint[i] = hex_to_printable(data[i]);
    }
    hex_write_string(dataPrint);

    numDataRead = hex_read(data);
    placeCount += 16;
    hex_format_offset(placeCount, place);
    hex_write_string("\n");

  }
    return 0;
}
