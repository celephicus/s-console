#ifndef CONSOLE_CONFIG_LOCAL_H__
#define CONSOLE_CONFIG_LOCAL_H__

// This is the default configuration file. It may be used as a template for a local configuration file `console-locals.h' which is included if `CONSOLE_USE_LOCALS' is defined.

// Our little language only works with one integral type, called a "cell" like FORTH. Usually it is the natural int for the part. For Arduino this is 16 bits. 
// Define to allow different types for the 'cell'. console_cell_t must be a signed type, console_ucell_t must be unsigned, and they must both be the same size.
#include <stdint.h>
typedef int32_t  sc_cell_t;
typedef uint32_t sc_ucell_t;

// For unit test we want printf format for signed cell.
#define SC_FORMAT_CELL "ld"

// User stack size, we don't need much.
#define SC_U_STACK_SIZE (8)
#define SC_R_STACK_SIZE (4)

// Input buffer size
#define SC_INPUT_BUFFER_SIZE 40

// Character to signal EOL for input string.
#define SC_INPUT_NEWLINE_CHAR '\r'

#endif // CONSOLE_CONFIG_LOCAL_H__
