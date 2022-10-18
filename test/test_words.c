#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "unity.h"

#define SCONSOLE_WANT_INTERNAL_DEFS
#include "sconsole.h"

#include "t_support.h"
TT_BEGIN_FIXTURE(tsSetupVmFixture, tsDumpVmFixture, tsDestroyVmFixture);


// Helpers for unary ops.
void testWordsUnop(uint16_t w, sc_cell_t a, sc_cell_t r) { // Verify unary op result.
	TS_ASSEMBLER_CALL(w);
	u_push(a);
	while (r_depth() > 0)
		TEST_ASSERT_EQUAL(SC_FAULT_OK, scRun(1));	
	tsAssertUStack(1, r);
}
TT_TEST_CASE(testWordsUnop(SC_WORD_1_PLUS, 0, 1));