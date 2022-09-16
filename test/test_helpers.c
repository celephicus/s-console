#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "unity.h"

TT_BEGIN_INCLUDE()
#define SCONSOLE_WANT_INTERNAL_DEFS
#include "sconsole.h"
TT_END_INCLUDE()

#include "t_support.h"

TT_BEGIN_FIXTURE(ts_SetupStackTestContext, NULL, ts_DestroyStackTestContext);

// Byte access to RAM heap.
void testHeapRamByte(sc_cell_t addr) {
	scHeapWriteByte(0, 123);
	TEST_ASSERT_EQUAL(SC_FAULT_OK, FAULT);
	TEST_ASSERT_EQUAL_UINT8(123, scHeapReadByte(0));
	TEST_ASSERT_EQUAL(SC_FAULT_OK, FAULT);
}
TT_TEST_CASE(testHeapRamByte(0));
TT_TEST_CASE(testHeapRamByte(SC_HEAP_SIZE-1));

void testHeapRamByteBadRead() {
	scHeapWriteByte(SC_HEAP_SIZE, 123);
	TEST_ASSERT_EQUAL(SC_FAULT_MEM, FAULT);
	FAULT = SC_FAULT_OK;
}
void testHeapRamByteBadWrite() {
	(void)scHeapReadByte(SC_HEAP_SIZE);
	TEST_ASSERT_EQUAL(SC_FAULT_MEM, FAULT);
	FAULT = SC_FAULT_OK;
}

// Cell access to RAM heap.
void testHeapRamCell(sc_cell_t addr) {
	scHeapWriteCell(0, 0x1234);
	TEST_ASSERT_EQUAL(SC_FAULT_OK, FAULT);
	TEST_ASSERT_EQUAL_SC_CELL(0x1234, scHeapReadCell(0));
	TEST_ASSERT_EQUAL(SC_FAULT_OK, FAULT);
}
TT_TEST_CASE(testHeapRamCell(0));
TT_TEST_CASE(testHeapRamCell(1));	// Check alignment.
TT_TEST_CASE(testHeapRamCell(SC_HEAP_SIZE-sizeof(sc_cell_t)));

void testHeapRamCellBadRead() {
	scHeapWriteCell(SC_HEAP_SIZE-sizeof(sc_cell_t)+1, 0x1234);
	TEST_ASSERT_EQUAL(SC_FAULT_MEM, FAULT);
	FAULT = SC_FAULT_OK;
}
void testHeapRamCellBadWrite() {
	(void)scHeapReadCell(SC_HEAP_SIZE-sizeof(sc_cell_t)+1);
	TEST_ASSERT_EQUAL(SC_FAULT_MEM, FAULT);
	FAULT = SC_FAULT_OK;
}

