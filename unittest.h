/*const uint16_t UT_BITS_ADDRESS = 0x7530;
const uint16_t UT_BITS_NB = 0x01;
const uint8_t UT_BITS_TAB[] = { 0x0A };

const uint16_t UT_INPUT_BITS_ADDRESS = 0x2710;
const uint16_t UT_INPUT_BITS_NB = 0x1;
const uint8_t UT_INPUT_BITS_TAB[] = { 0x05 };

const uint16_t UT_REGISTERS_ADDRESS = 0x9C40;
const uint16_t UT_REGISTERS_NB = 0x04;
const uint16_t UT_REGISTERS_NB_MAX = 0x20;
const uint16_t UT_REGISTERS_TAB[] = { 0xCD, 0x6B};

const uint16_t UT_INPUT_REGISTERS_ADDRESS = 0x4E20;
const uint16_t UT_INPUT_REGISTERS_NB = 0x04;
const uint16_t UT_INPUT_REGISTERS_TAB[] = {  0xB2, 0x0E};*/


#ifndef _UNIT_TEST_H_
#define _UNIT_TEST_H_

/* Constants defined by configure.ac */
#define HAVE_INTTYPES_H @HAVE_INTTYPES_H@
#define HAVE_STDINT_H @HAVE_STDINT_H@

#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#ifdef HAVE_STDINT_H
# ifndef _MSC_VER
# include <stdint.h>
# else
# include "stdint.h"
# endif
#endif

#define SERVER_ID         17
#define INVALID_SERVER_ID 18
const uint16_t UT_BITS_ADDRESS = 0x7530;
const uint16_t UT_BITS_NB = 0x25;
const uint8_t UT_BITS_TAB[] = { 0xCD, 0x6B, 0xB2, 0x0E, 0x1B };

const uint16_t UT_INPUT_BITS_ADDRESS = 0x2710;
const uint16_t UT_INPUT_BITS_NB = 0x16;
const uint8_t UT_INPUT_BITS_TAB[] = { 0xAC, 0xDB, 0x35 };

const uint16_t UT_REGISTERS_ADDRESS = 0x9C40;
const uint16_t UT_REGISTERS_NB = 0x3;
const uint16_t UT_REGISTERS_NB_MAX = 0x20;
const uint16_t UT_REGISTERS_TAB[] = { 0x022B, 0x0001, 0x0064 };

const uint16_t UT_INPUT_REGISTERS_ADDRESS = 0x4E20;
const uint16_t UT_INPUT_REGISTERS_NB = 0x1;
const uint16_t UT_INPUT_REGISTERS_TAB[] = { 0x000A };



/* Raise a manual exception when this address is used for the first byte */
const uint16_t UT_REGISTERS_ADDRESS_SPECIAL = 0x170;
/* The response of the server will contains an invalid TID or slave */
const uint16_t UT_REGISTERS_ADDRESS_INVALID_TID_OR_SLAVE = 0x171;
/* The server will wait for 1 second before replying to test timeout */
const uint16_t UT_REGISTERS_ADDRESS_SLEEP_500_MS = 0x172;
/* The server will wait for 5 ms before sending each byte */
const uint16_t UT_REGISTERS_ADDRESS_BYTE_SLEEP_5_MS = 0x173;

/* If the following value is used, a bad response is sent.
   It's better to test with a lower value than
   UT_REGISTERS_NB_POINTS to try to raise a segfault. */
const uint16_t UT_REGISTERS_NB_SPECIAL = 0x2;



const float UT_REAL = 123456.00;

const uint32_t UT_IREAL_ABCD = 0x0020F147;
const uint32_t UT_IREAL_DCBA = 0x47F12000;
const uint32_t UT_IREAL_BADC = 0x200047F1;
const uint32_t UT_IREAL_CDAB = 0xF1470020;

/* const uint32_t UT_IREAL_ABCD = 0x47F12000);
const uint32_t UT_IREAL_DCBA = 0x0020F147;
const uint32_t UT_IREAL_BADC = 0xF1470020;
const uint32_t UT_IREAL_CDAB = 0x200047F1;*/

#endif /* _UNIT_TEST_H_ */
