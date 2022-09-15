#ifndef CONSOLE_CONFIG_LOCAL_H__
#define CONSOLE_CONFIG_LOCAL_H__

// This is the default configuration file. It may be used as a template for a local configuration file `console-locals.h' which is included if `CONSOLE_USE_LOCALS' is defined.

// Our little language only works with one integral type, called a "cell" like FORTH. Usually it is the natural int for the part. For Arduino this is 16 bits. 
// Define to allow different types for the 'cell'. Note that console_cell_t must be a signed type, console_ucell_t must be unsigned, and they must both be
//  the same size.
#include <stdint.h>
typedef int32_t  sc_cell_t;
typedef uint32_t sc_ucell_t;

// Stack size, we don't need much.
#define SC_U_STACK_SIZE (8)
#define SC_R_STACK_SIZE (4)

// Heap space in RAM, again we don't need much, 256 bytes is a good start.
#define SC_HEAP_SIZE 256

// Read only area in const 
#define SC_READ_ONLY_PHYS_START
#define SC_READ_ONLY_SIZE 

// For test suite we need a checker for cell values.
#define TEST_ASSERT_EQUAL_SC_CELL TEST_ASSERT_EQUAL_INT32

#if 0
// For unit test we want printf format for signed cell.
#define CONSOLE_FORMAT_CELL "ld"

// Input buffer size
#define CONSOLE_INPUT_BUFFER_SIZE 40

// Character to signal EOL for input string.
#define CONSOLE_INPUT_NEWLINE_CHAR '\r'
#endif

#endif // CONSOLE_CONFIG_LOCAL_H__
