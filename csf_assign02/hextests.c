// Unit tests for hex functions
// These tests should work for both your C implementations and your
// assembly language implementations

#include "hexfuncs.h"
#include "tctest.h"
#include <stdio.h>
#include <stdlib.h>

// test fixture object
typedef struct {
  char test_data_1[16];
} TestObjs;

// setup function (to create the test fixture)
TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));
  strcpy(objs->test_data_1, "Hello, world!\n");
  return objs;
}

// cleanup function (to destroy the test fixture)
void cleanup(TestObjs *objs) { free(objs); }

// Prototypes for test functions
void testFormatOffset(TestObjs *objs);
void testFormatByteAsHex(TestObjs *objs);
void testHexToPrintable(TestObjs *objs);

int main(int argc, char **argv) {
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(testFormatOffset);
  TEST(testFormatByteAsHex);
  TEST(testHexToPrintable);

  TEST_FINI();

  return 0;
}

void testFormatOffset(TestObjs *objs) {
  (void)objs; // suppress warning about unused parameter
  char buf[16];
  hex_format_offset(0x00000001u, buf);
  ASSERT(0 == strcmp(buf, "00000001"));
  hex_format_offset(0xabcd1234u, buf);
  ASSERT(0 == strcmp(buf, "abcd1234"));
  hex_format_offset(0x00000000u, buf);
  ASSERT(0 == strcmp(buf, "00000000"));
  hex_format_offset(0xffffffffu, buf);
  ASSERT(0 == strcmp(buf, "ffffffff"));
  hex_format_offset(0xfedcba98u, buf);
  ASSERT(0 == strcmp(buf, "fedcba98"));
}

void testFormatByteAsHex(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[0], buf);
  ASSERT(0 == strcmp(buf, "48"));
  hex_format_byte_as_hex(objs->test_data_1[1], buf);
  ASSERT(0 == strcmp(buf, "65"));
  hex_format_byte_as_hex(objs->test_data_1[2], buf);
  ASSERT(0 == strcmp(buf, "6c"));
  hex_format_byte_as_hex(objs->test_data_1[3], buf);
  ASSERT(0 == strcmp(buf, "6c"));
  hex_format_byte_as_hex(objs->test_data_1[4], buf);
  ASSERT(0 == strcmp(buf, "6f"));
  hex_format_byte_as_hex(objs->test_data_1[5], buf);
  ASSERT(0 == strcmp(buf, "2c"));
  hex_format_byte_as_hex(objs->test_data_1[6], buf);
  ASSERT(0 == strcmp(buf, "20"));
  hex_format_byte_as_hex(objs->test_data_1[7], buf);
  ASSERT(0 == strcmp(buf, "77"));
  hex_format_byte_as_hex(objs->test_data_1[8], buf);
  ASSERT(0 == strcmp(buf, "6f"));
  hex_format_byte_as_hex(objs->test_data_1[9], buf);
  ASSERT(0 == strcmp(buf, "72"));
  hex_format_byte_as_hex(objs->test_data_1[10], buf);
  ASSERT(0 == strcmp(buf, "6c"));
  hex_format_byte_as_hex(objs->test_data_1[11], buf);
  ASSERT(0 == strcmp(buf, "64"));
  hex_format_byte_as_hex(objs->test_data_1[12], buf);
  ASSERT(0 == strcmp(buf, "21"));
}

void testHexToPrintable(TestObjs *objs) {
  // test all letters inside "Hello, world!"
  ASSERT('H' == hex_to_printable(objs->test_data_1[0]));
  ASSERT('e' == hex_to_printable(objs->test_data_1[1]));
  ASSERT('l' == hex_to_printable(objs->test_data_1[2]));
  ASSERT('l' == hex_to_printable(objs->test_data_1[3]));
  ASSERT('o' == hex_to_printable(objs->test_data_1[4]));
  ASSERT(',' == hex_to_printable(objs->test_data_1[5]));
  ASSERT(' ' == hex_to_printable(objs->test_data_1[6]));
  ASSERT('w' == hex_to_printable(objs->test_data_1[7]));
  ASSERT('o' == hex_to_printable(objs->test_data_1[8]));
  ASSERT('r' == hex_to_printable(objs->test_data_1[9]));
  ASSERT('l' == hex_to_printable(objs->test_data_1[10]));
  ASSERT('d' == hex_to_printable(objs->test_data_1[11]));
  ASSERT('!' == hex_to_printable(objs->test_data_1[12]));
  ASSERT('.' == hex_to_printable(objs->test_data_1[13]));
}
